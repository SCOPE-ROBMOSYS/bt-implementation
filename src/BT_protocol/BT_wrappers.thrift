/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/

 /**
 * Enumerates the states every node can be in after execution during a particular
 * time step:
 * - "SKILL_SUCCESS" indicates that the node has completed running during this time step;
 * - "SKILL_FAILURE" indicates that the node has determined it will not be able to complete
 *   its task;
 * - "SKILL_RUNNING" indicates that the node has successfully moved forward during this
 *   time step, but the task is not yet complete;
 * - "SKILL_IDLE" indicates that the node hasn't run yet.
 */
enum SkillAck {SKILL_IDLE, SKILL_RUNNING, SKILL_SUCCESS, SKILL_FAILURE, BT_UNDEFINED}

service Skill_request {

    /**
     * request_status  Get the status of the action on the server side.
     *
     * return              The enum indicating the status of the action on the server side.
     */
    SkillAck request_ack();

    /**
     * request_tick  Send a Tick request to the server, along with its parameters.
     *
     * return               The enum indicating the status of the action on the server side.
     */
    SkillAck request_tick  ();

    /**
     * request_halt  Send a Halt request to the server, along with its parameters.
     *
     * return              void.
     */
     void request_halt  ();
}
