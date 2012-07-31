/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"

using namespace mbeam_imagenex;

Task::Task(std::string const& name)
    : TaskBase(name),mDriver(0)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine),mDriver(0)
{
}

Task::~Task()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook()
{
   try{
      LOG_DEBUG("configureHook");
      delete mDriver;
      mbeam_imagenex::Config configuration = _config.get();
      mDriver = new mbeam_imagenex::Driver(configuration);
      if (!_io_port.get().empty())
      {
	  mDriver->open(_io_port.get());
      }
      setDriver(mDriver);
      if (! TaskBase::configureHook())
	  return false;
      return true;
    } catch(std::runtime_error &e){
      LOG_DEBUG("exception %s",e.what());
      error(COMM_ERROR);      
      return false;
    } 
}

bool Task::startHook()
{
   try{
      LOG_DEBUG("start	Hook");
      
      if (! TaskBase::startHook())
	  return false;
      return true;
    } catch(std::runtime_error &e){
      LOG_DEBUG("exception %s",e.what());
      error(COMM_ERROR);      
      return false;
    } 
}

void Task::updateHook()
{
  try{
    mDriver->sendExtCmd();
    mDriver->collectData();
    _mbeam_scan.write(mDriver->getData());
    TaskBase::updateHook();
  } catch(std::runtime_error &e){
    LOG_DEBUG("exception %s",e.what());
    error(COMM_ERROR);      
  }
}


void Task::errorHook()
{
    TaskBase::errorHook();
    recover();

}

void Task::stopHook()
{
    TaskBase::stopHook();
}

void Task::cleanupHook()
{
  if(mDriver){
    mDriver->close();
  }  
  TaskBase::cleanupHook();
}

