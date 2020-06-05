roscore &
sleep 1;
yarp server --ros --write&
sleep 1;
gazebo --verbose ./r1world.world &
