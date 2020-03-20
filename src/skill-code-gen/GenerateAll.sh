rm -r /home/scope/bt-implementation/src/BatteryLevel_skill &&
rm -r /home/scope/bt-implementation/src/BatteryNotCharging_skill &&
rm -r /home/scope/bt-implementation/src/GoTo_skill &&
cd /home/scope/bt-implementation/build/bin &&
./skill-code-gen /home/scope/bt-implementation/src/skill-code-gen/input-files/BatteryLevelSkillStateMachine.scxml &&
./skill-code-gen /home/scope/bt-implementation/src/skill-code-gen/input-files/BatteryNotChargingSkillStateMachine.scxml &&
./skill-code-gen /home/scope/bt-implementation/src/skill-code-gen/input-files/GoToSkillStateMachine.scxml
