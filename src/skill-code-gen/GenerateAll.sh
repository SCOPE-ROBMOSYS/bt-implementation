rm -r /home/scope/bt-implementation/src/skill-code-gen/output-files &&
cd /home/scope/bt-implementation/build/bin &&
./skill-code-gen /home/scope/bt-implementation/src/skill-code-gen/input-files/BatteryLevelSkillStateMachine.scxml &&
./skill-code-gen /home/scope/bt-implementation/src/skill-code-gen/input-files/BatteryNotChargingSkillStateMachine.scxml &&
./skill-code-gen /home/scope/bt-implementation/src/skill-code-gen/input-files/GoToSkillStateMachine.scxml
