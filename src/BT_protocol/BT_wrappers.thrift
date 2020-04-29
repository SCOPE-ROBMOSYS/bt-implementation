/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/


enum SkillAck {SKILL_IDLE, SKILL_RUNNING, SKILL_SUCCESS, SKILL_FAILURE, SKILL_STARTED, SKILL_STOPPED}

service Skill_request {

    /**
     * request_ack  Get the ack of the skill.
     *
     * return              The enum indicating the ack.
     */
    SkillAck request_ack();

    /**
     * send_start  Send a CMD_START sto the skill.
     *
     * return               The enum indicating the status of the action on the server side.
     */
    void send_start();

    /**
     * send_stop  Send a CMD_STOP request to the skill.
     *
     * return              void.
     */
     void send_stop();
}
