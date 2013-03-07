/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <base_schilling/Error.hpp>

using namespace mbeam_imagenex;
using namespace oro_marum;

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
}

bool Task::startHook()
{
  if (! TaskBase::startHook())
    return false;
  return true;
}

void Task::updateHook()
{
  try{
    switch(state()){
      case RUNNING: {
	processIO();
	state(MONITORING);
	break;
      }
      case MONITORING:{
	processIO();
	break;
      }
      default: break;
    }      
  } catch(std::runtime_error &e){
    LOG_DEBUG("exception %s",e.what());
    _log_message.write(LogMessage(e));
    exception(IO_TIMEOUT);  
  }
}


void Task::errorHook()
{
    LOG_DEBUG("errorHook");
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

void Task::processIO()
{
    int val;
    if (_gain.read(val) == RTT::NewData) {
       mDriver->setGain(val);
    }      
    if (_range.read(val) == RTT::NewData) {
       mDriver->setRange(val);
    }
    mDriver->collectData();
    mDriver->sendExtCmd();
    _mbeam_scan.write(mDriver->getData());
}
