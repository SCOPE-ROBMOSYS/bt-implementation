enum GoToStatus
{
    NOT_STARTED,
    RUNNING,
    SUCCESS,
    ABORT
}

service GoTo {
  void goTo(1: string destination);
  GoToStatus getStatus(1: string destination);
  void halt(1: string destination);
}
