/**
 * Project Untitled
 */


#ifndef _MYTASK_H
#define _MYTASK_H

#include "Task.h"



class myTask: public Task {
public: 
    
virtual void process() override;
};

#endif //_MYTASK_H