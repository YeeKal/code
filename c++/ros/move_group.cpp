/************************
*move group for nachi-mz04
*Author: yee
*Date: 2018-03-31
*************************/

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>

#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>

#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>

#include <moveit_visual_tools/moveit_visual_tools.h>

int main(int argc,char **argv){
    ros::init(argc,argv,"move_group_mz04");
    ros::NodeHandle node_handle;
    ros::AsyncSpinner spinner(1);
    spinner.start();

    static std::string PLANNING_GROUP="arm";
    moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP);
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
     

    robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
    robot_model::RobotModelPtr kinematic_model = robot_model_loader.getModel();
    robot_state::RobotStatePtr kinematic_state(new robot_state::RobotState(kinematic_model));
    const robot_state::JointModelGroup *joint_model_group =move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP);
 
    namespace rvt=rviz_visual_tools;
    moveit_visual_tools::MoveItVisualTools visual_tools("world");
    visual_tools.deleteAllMarkers();
    visual_tools.loadRemoteControl();
    Eigen::Affine3d text_pose=Eigen::Affine3d::Identity();
    text_pose.translation().z()=0.75;
    visual_tools.publishText(text_pose,"nachi-mz04 motion planning", rvt::WHITE, rvt::XLARGE);
    visual_tools.trigger();
    visual_tools.prompt("next step");

    //define a collision object
    moveit_msgs::CollisionObject collision_object;
    collision_object.header.frame_id=move_group.getPlanningFrame();
    collision_object.id="cone";

    //define a box shape
    shape_msgs::SolidPrimitive primitive;
    primitive.type=primitive.CONE;
    primitive.dimensions.resize(2);
    primitive.dimensions[0]=0.5;
    primitive.dimensions[1]=0.3;
    geometry_msgs::Pose cone_pose;
    cone_pose.orientation.w=1;
    cone_pose.position.x=0.5;
    cone_pose.position.y=-0.5;
    cone_pose.position.z=0.3;

    collision_object.primitives.push_back(primitive);
    collision_object.primitive_poses.push_back(cone_pose);
    collision_object.operation=collision_object.ADD;

    std::vector<moveit_msgs::CollisionObject> collision_objects;
    collision_objects.push_back(collision_object);

    ROS_INFO("add an object into the world.");
    planning_scene_interface.addCollisionObjects(collision_objects);
    ros::Duration(1.0).sleep();


    //1. plan to a pose goal
    geometry_msgs::Pose target_pose1;
    target_pose1.orientation.x= 1.0;
    target_pose1.position.x = 0.3;
    target_pose1.position.y = -0.2;
    target_pose1.position.z = 0.4;

    move_group.setPoseTarget(target_pose1);

    bool success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);
    ROS_INFO_NAMED("tutorial", "Pose goal plan: %s", success ? "SUCCESSED" : "FAILED");
    
    if(success){
        ROS_INFO("Visualizing pose goal plan.");
        move_group.execute(my_plan);
    }
    
    visual_tools.publishAxisLabeled(target_pose1, "pose1");//coordinates with 3 axis
    visual_tools.publishText(text_pose,"Pose Goal", rvt::WHITE,rvt::XLARGE);
    visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
    visual_tools.trigger();
    visual_tools.prompt("next");//its feasible

    //plan2 change one joint
    std::vector<double> joint_group_values;
    move_group.getCurrentState()->copyJointGroupPositions(joint_model_group,joint_group_values);
    const std::vector<std::string> &joint_names = joint_model_group->getVariableNames();
    for (std::size_t i = 0; i < joint_names.size(); ++i)
    {
        ROS_INFO("Joint %s: %f", joint_names[i].c_str(), joint_group_values[i]);
    }
    joint_group_values[3]  +=1;
    //visual_tools.prompt("next");
    move_group.setJointValueTarget(joint_group_values);

    success = (move_group.plan(my_plan) == moveit::planning_interface::MoveItErrorCode::SUCCESS);

    ROS_INFO_NAMED("tutorial", "Joint values plan: %s", success ? "SUCCESSED" : "FAILED");
    
    if(success){
        ROS_INFO("Visualizing joint values plan.");
        move_group.execute(my_plan);
    }

    visual_tools.deleteAllMarkers();
    visual_tools.publishText(text_pose,"Joint Goal", rvt::WHITE,rvt::XLARGE);
    visual_tools.trigger();
    visual_tools.prompt("next");

    //plan3 cartesian path plan
    std::vector<geometry_msgs::Pose> waypoints;
    geometry_msgs::Pose startPose2;
    startPose2=target_pose1;  //move_group.getCurrentPose().pose();
    waypoints.push_back(startPose2);

    geometry_msgs::Pose target_pose2=startPose2;
    target_pose2.position.y=0.1;
    waypoints.push_back(target_pose2);

    target_pose2.position.z=0.6;
    waypoints.push_back(target_pose2);

    target_pose2.position.y=-0.1;
    waypoints.push_back(target_pose2);

    waypoints.push_back(startPose2);

    move_group.setMaxVelocityScalingFactor(0.2);
    moveit_msgs::RobotTrajectory trajectory;
    const double jump_threshold = 0.0;
    const double eef_step = 0.01;
    double fraction = move_group.computeCartesianPath(waypoints, eef_step, jump_threshold, trajectory);
    ROS_INFO_NAMED("tutorial", "Visualizing cartesian plan (%.2f%% acheived)", fraction * 100.0);

    visual_tools.deleteAllMarkers();
    visual_tools.publishText(text_pose,"cartesian goal",rvt::WHITE,rvt::XLARGE);
    visual_tools.publishPath(waypoints,rvt::LIME_GREEN,rvt::SMALL);
    for (std::size_t i = 0; i < waypoints.size(); ++i)
        visual_tools.publishAxisLabeled(waypoints[i], "pt" + std::to_string(i), rvt::SMALL);
    visual_tools.trigger();
    visual_tools.prompt("next");

    visual_tools.deleteAllMarkers();

    my_plan.trajectory_=trajectory;
    move_group.execute(my_plan);

    std::vector<std::string> object_ids;
    object_ids.push_back(collision_object.id);
    planning_scene_interface.removeCollisionObjects(object_ids);
    visual_tools.deleteAllMarkers();

    ros::shutdown();
    return 0;
}