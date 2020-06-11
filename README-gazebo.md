Compile the image containing Gazebo:

docker-compose build navigation-gazebo

1 : Open a new terminal.

$ sudo xhost +
$ nvidia-docker run --rm -it --gpus all -e QT_X11_NO_MITSHM=1 -e DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -e NVIDIA_DRIVER_CAPABILITIES=compute,utility,graphics --hostname dockerpc --net="host" scoperobmosys/navigation-gazebo

You should be able to see the ports of the other docker containers, being the network set to the host machine.
Run the Gazebo application:

$ ./startup_commands.sh

2 : Open another terminal. Run the whole application, with the 2d simulator

$ cd bt-implementation
$ docker-compose up

Simulation:

The Gazebo simulation starts and the cer robot moves according to the signal received in yarp port:
 /baseControl/control:i

Other comments:

- In case the command
$ yarp name list
does not show the ports of the main application, you can re-configure the yarp server using:
$ yarp conf --clean
$ yarp detect --write

- In case you wish to differentiate the port names, you can use: (however it is not necessary)
export YARP_PORT_PREFIX=/gazebo

- To generate the 3D heightmap for Gazebo, you can run:
$ yarp server
$ cd /navigation/build/bin
$ ./map2Gazebo --from_file maps_SCOPE/map_test.map --ceiling 2

- In order to check that gazebo is able to include the map, run:
printenv GAZEBO_MODEL_PATH
