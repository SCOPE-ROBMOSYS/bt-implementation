/******************************************************************************
 *                                                                            *
 * Copyright (C) 2020 Fondazione Istituto Italiano di Tecnologia (IIT)        *
 * All Rights Reserved.                                                       *
 *                                                                            *
 ******************************************************************************/



service ArmService {
    bool preGrasp();
    bool extractHand();
    bool retractHand();
    bool closeHand();
    bool openHand();
    bool hasGrasped();
    bool home();
}
