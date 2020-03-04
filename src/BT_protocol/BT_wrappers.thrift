/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

 /**
 * Enumerates the states every node can be in after execution during a particular
 * time step:
 * - "BT_SUCCESS" indicates that the node has completed running during this time step;
 * - "BT_FAILURE" indicates that the node has determined it will not be able to complete
 *   its task;
 * - "BT_RUNNING" indicates that the node has successfully moved forward during this
 *   time step, but the task is not yet complete;
 * - "BT_IDLE" indicates that the node hasn't run yet.
 */
enum ReturnStatus {BT_IDLE, BT_RUNNING, BT_SUCCESS, BT_FAILURE, BT_UNDEFINED}

service BT_request {

    /**
     * request_status  Get the status of the action on the server side.
     *
     * return              The enum indicating the status of the action on the server side.
     */
    ReturnStatus request_status();

    /**
     * request_tick  Send a Tick request to the server, along with its parameters.
     *
     * return               The enum indicating the status of the action on the server side.
     */
    ReturnStatus request_tick  ();

    /**
     * request_halt  Send a Halt request to the server, along with its parameters.
     *
     * return              void.
     */
     void request_halt  ();
}
