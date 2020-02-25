enum GoToStatus
{
    NOT_STARTED,
    RUNNING,
    SUCCESS,
    ABORT
}

service GoTo {
  void goTo();
  GoToStatus getStatus();
  void halt();
}
