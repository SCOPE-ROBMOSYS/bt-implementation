# Behavior Trees Implementation 

Repository containing the software to run the experimental validation's scenario.




## Dependencies

- [Docker](https://www.docker.com/)
- [YARP](https://www.yarp.it/) (Optional)



## Setup (Tested with Ubuntu 18.04 and Debian Testing)



1. **Install Docker**
   
Follow the instructions at https://docs.docker.com/install/
   
2. **install Docker Compose**

   Open a terminal window and run:

   

   `sudo curl -L "https://github.com/docker/compose/releases/download/1.25.4/docker-compose-$(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose`

   

   `sudo chmod +x /usr/local/bin/docker-compose`

   

   `sudo usermod -aG docker $USER`

3. **Reboot your system**

4. **Download the software to run the experiment**

   Open a terminal window and run:
   `git clone https://github.com/SCOPE-ROBMOSYS/bt-implementation.git`

   `cd bt-implementation`

   `docker-compose pull yarp-mesa navigation qscxml`

   `docker-compose build bt-implementation`

   If YARP  is not installed in the system then run:

   `mkdir -p ${HOME}/.config/yarp/ ${HOME}/.local/share/yarp/`

   `docker-compose run -T --rm yarp-mesa yarp conf <ip address> 10000`

### Execution

Open a terminal  window and the run

`docker-compose up`

Several windows will open as in the figure below. 

![scope-up](https://user-images.githubusercontent.com/8132627/77664063-18bfb200-6f7e-11ea-8da2-5ed9f36f358c.png)

The experiment starts as soon as all the components are up an running.



To change  the battery level, open a terminal window in the folder `bt-implementation`  and run

`docker-compose run --rm yarp-mesa yarp rpc /fakeBattery/control/rpc:i`

then write

`SetBatteryCharge X`

with `X` the new value of the battery level (between 0 an 100), as below:

![ezgif-7-b0a8d90c3f74](https://user-images.githubusercontent.com/8132627/77671740-50cbf280-6f88-11ea-9efb-769de0913f9d.gif)







