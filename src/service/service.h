/*
  Here one may find all of the
  declarations for the structs
  and methods necessary for 
  running the dugit background
  service.

  For the time being, this is
  not a priority for dugit to
  function manually. Later, I
  am thinking to create a
  boot-up service to revive
  unfinished cron jobs,
  regarding scheduled autosync
  and obviously implied, to use
  crontab for running autosync
  jobs.

  It's either this, or I will
  create a single constantly
  running service that will 
  do these autosync jobs itself,
  without crontab.  
*/

// service.h
#ifndef SERVICE_H
#define SERVICE_H

#include "include.h"

#endif