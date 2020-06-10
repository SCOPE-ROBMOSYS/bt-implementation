// 0

docker-compose build navigation-gazebo

//1
docker-compose up

//2
TERMINALE 1
sudo xhost +
nvidia-docker run --rm -it --gpus all -e QT_X11_NO_MITSHM=1 -e DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -e NVIDIA_DRIVER_CAPABILITIES=compute,utility,graphics --hostname dockerpc --net="host" scoperobmosys/navigation-gazebo
export YARP_PORT_PREFIX=/gazebo
yarp conf --clean
yarp detect --write
yarp name list

./startup_commands.sh

TERMINALE 2
sudo xhost +
nvidia-docker run --rm -it --gpus all -e QT_X11_NO_MITSHM=1 -e DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -e NVIDIA_DRIVER_CAPABILITIES=compute,utility,graphics --hostname dockerpc --net="host" scoperobmosys/navigation-gazebo
export YARP_PORT_PREFIX=/gazebo
yarp conf --clean
yarp detect --write
yarp name list




yarp read /read




MAP CREATE 3D

yarp server
/workspaces-iit/Navigation/master/navigation/build/bin$      ./map2Gazebo --from_file maps_SCOPE/map_test.map --ceiling 2


GAZEBO

printenv GAZEBO_MODEL_PATH
