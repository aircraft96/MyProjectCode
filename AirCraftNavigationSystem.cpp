//Project A: Aicraft Navigation System
//December 9,2017



//headers needed
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <queue>
#include <stdlib.h>
#include <algorithm> 
using namespace std;

//forward declaration
class AircraftScheduler;
class AircraftMonitoringSystem;

#define NUMBER_OF_TASKS 5;
int total_Tasks = NUMBER_OF_TASKS;

class AirCraftTasks
{

public:
    //define the core characteristics of the aircraft navigation system tasks
	string task_Name; //task name
	chrono::milliseconds task_Period_Time; //period of task
	chrono::milliseconds task_Execution_Time; //execution time
	chrono::milliseconds task_Deadline_Time; //deadline time

	//other critical
	bool task_Initiation; //is task active?
	long task_Instance_Number; //task instance
	long task_Worst_Case_Deadline; //time in secs before which the task must complete
	int taskPriority;
public:

	queue<int> executed_Tasks;//contains all of the executed tasks
	queue<int> executing_Tasks;//contains all of the executing tasks

	int task_Status;//state of task executing or idle for interuption

	//current deadline of task: needs to be set by system clock
	chrono::system_clock::time_point task_Current_Deadline;
	string warning_Display_Message; //message which will be in the display screen

	//Default Constructor
	AirCraftTasks()
	{};

	//AirCraftTasks Constructor
	AirCraftTasks (string name,
				   chrono::milliseconds &period,
			       chrono::milliseconds &execution,
			       chrono::milliseconds &deadline,
			       string w_message)
	{
		task_Name = name;
		task_Period_Time = period;
		task_Execution_Time = execution;
		task_Deadline_Time = deadline;

		//default initialization
		task_Initiation = false;
		task_Instance_Number = 0;
		task_Worst_Case_Deadline = 0;

		//warning message to display
		warning_Display_Message = w_message;
	}

	void execute_AirCraftTasks(std::chrono::system_clock::time_point current_Time)
	{
		bool task_Idle; // is task active?

		//loop termination condition for a task
		if ((task_Current_Deadline - task_Deadline_Time) > current_Time)
		    return;

		//find the Current Time when task will start to execute
		current_Time = current_Time + task_Execution_Time;

		//determine if task can be scheduled
		if (current_Time < task_Current_Deadline)
		{
			if (executing_Tasks.size() == 0)
			{
				task_Worst_Case_Deadline = std::rand() % 50;
			}
			else
			{
				task_Worst_Case_Deadline = executing_Tasks.front();

				//task not scheduled
			    task_Idle = true;
			}
		}
		else
		{
			//sleepUntil: stops execution of the current thread till value.
			std::this_thread::sleep_until(task_Current_Deadline);
			
			//Task is scheduled
	        task_Idle = false;
		}

		//define the new current deadline of the task
		 task_Current_Deadline = task_Current_Deadline + task_Deadline_Time;  
		
		 //remember to increment the instanc number of task
		 task_Instance_Number = task_Instance_Number + 1;

		 if(task_Instance_Number < -1)
			 cout<< "Error in the instance number of the task. Please check initialization.";

		 if (executing_Tasks.empty())
			 cout<< "\nExecuting Tasks queue.\n";

		 else 
		 {
			 //remove an task instance from the queue
			  executing_Tasks.pop();
		 }
  
		//Push the executed task into the queue of executed tasks
		 executed_Tasks.push(task_Worst_Case_Deadline);		 		 
   		 this_thread::sleep_until(current_Time);	
		
	}
  
	void define_Current_Deadline(chrono::system_clock::time_point const& currentValue)
	{
		task_Current_Deadline = task_Deadline_Time + currentValue;
	}

};

class AircraftScheduler
{
	//Definition of STL method:
	// --> chrono::time_point represents a point in time

private:
	//contains the private data members of the class AircraftScheduler
	chrono::system_clock::time_point controlTime; // used for timing control in the scheduler	

public:

	//a double-sided queue with all the tasks
	deque<AirCraftTasks*> list_of_AC_Tasks;
	
	//critical functions for the class Scheduler
  void initiateScheduler()
  {  
	  auto ac_tasks_iterator =  list_of_AC_Tasks.begin();

	  while (ac_tasks_iterator != list_of_AC_Tasks.end())
	  {
		  (*ac_tasks_iterator)->define_Current_Deadline(chrono::system_clock::now());
		 
		  //sort the queue by the priority of the tasks
		  sort(list_of_AC_Tasks.begin(), list_of_AC_Tasks.end());

		  //increment the iterator
		  ac_tasks_iterator++;  	  
	  }
  };

  //Define the interupt handler tasks - mostly smoke warning tasks
  void InteruptHandlerTasks(AirCraftTasks* ac_Task)
  {
		//update the current deadline of the task
	  ac_Task->define_Current_Deadline(chrono::system_clock::now());
	  list_of_AC_Tasks.push_front(ac_Task);  
  };
  //Tasks Execution for the scheduler
  AirCraftTasks* Scheduler_Execution_Tasks()
  {
	  AirCraftTasks* ac_task = list_of_AC_Tasks.front();
	  list_of_AC_Tasks.pop_front();

	//execute the tasks in the AC_Tasks deque
		ac_task->execute_AirCraftTasks(chrono::system_clock::now());		
		return ac_task;
  }
};

class AircraftMonitoringSystem
{

private:
	//critical functions important for the functionality of the display of ACSystem
	AircraftScheduler ACScheduler_Object;
	

public:  
  AircraftMonitoringSystem()
  {};

  ~AircraftMonitoringSystem()
  {};
  
  bool system_Status;// system on or off
  
  void addTask(string taskName,
	           chrono::milliseconds* task_Deadline,
	           chrono::milliseconds* task_Execution_Time,
			   chrono::milliseconds* task_Period,
			   AirCraftTasks &ac_tasks)
  {
		//add the taask to the AircraftScheduler: We are adding the AC tasks to the scheduler
	  ACScheduler_Object.InteruptHandlerTasks(&ac_tasks); 
  
  }

  void ShowDialsofACSystem(string user_Command)
  {
	  for(int i = 0; i<5 ; i++)
	  {
		
		  if (user_Command == "Engine Temperature Sensor" )
			 { 
				 cout<< "\n [Dials Signal Screen Option]: Engine Temperature Sensor ";
				 break;
			 }
		  
			  if (user_Command == "Engine Pressure Sensor" )
			  {
				cout<< "\n [Dials Signal Screen Option]: Engine Pressure Sensor ";
				break;
			  }

			  if (user_Command == "Engine Fuel Sensor")
			  {
				  cout<< "\n [Dials Signal Screen Option]: Engine Fuel Sensor ";
				  break;
			  }

			  if (user_Command == "Smoke Detector 1 Sensor")
			  {
				  cout<< "\n [Dials Signal Screen Option]: Smoke Detector 1 Sensor ";
				  break;
			  }

			 if (user_Command == "Smoke Detector 2 Sensor")
			 {
				 cout<< "\n [Dials Signal Screen Option]: Smoke Detector 2 Sensor ";
				 break;
			 }
			 
	  }
  }

  void executePriorityScheduling()
  {
	// To implement this method, the following link was used as an reference:
	//thecrazyprogrammer [3]
	//definig the method variables
    int executionTime[5];//bt
	int task_InstanceNumber[5];

	//define the more method variables
	int taskWaitingTime[5];
	int turnAroundTime[5];
	int taskPriority[5];
	
	int total_Time_Calculated = 0;
	
	int index = 0;
	int position_Temporary;
	int average_WaiitingTime;
	int average_TurnaroundTime;

    cout<<"Set of Tasks to Excute in the AirCraft System:\n"<<NUMBER_OF_TASKS;
	cout<<endl;
	   
     cout<<"\nExecuting Aircraft Tasks based on Priority and Deadlines:\n";
    
	for(int i=0; i<total_Tasks ; i++)
    {
        cout<<"\nAircraft Task Number["<<i+1<<"]\n";

        cout<<"Execution Time (ms):";
        cin>>executionTime[i];
        cout<<"Aircraft System Tasks Priority:";
        cin>>taskPriority[i];
        task_InstanceNumber[i]=i+1;           //it is the Task Instance Number

    }
 
    //Place the processes in order of Priority, Execution Time, Deadline in the queue of AC Tasks
	
	for(int i = 0; i < total_Tasks; i++)
    {
		//sorting algorithm process
        index = i; //task index

		for(int j = i+1; j < total_Tasks; j++)
        {
            if(taskPriority[j] < taskPriority[index])
                index = j;
        }
 
		//performing the swaps in the queue of tasks to oder them
        position_Temporary = taskPriority[i];
        taskPriority[i] = taskPriority[index];

		//the new taskPriority is the position_Temporary
        taskPriority[index] = position_Temporary;
 
		//define the position_Temporary based on execution times
		position_Temporary = executionTime[i];

		//execution time is modified
		executionTime[i] = executionTime[index];

		//execution time is given the temmporary value
		executionTime[index] = position_Temporary;
 
		position_Temporary = taskPriority[i];

		//setting the priority of the tasks
		taskPriority[i] = taskPriority[index];
		taskPriority[index] = position_Temporary;
    }
 
	//defining the other characteristics of the process
	//define the waiting time of the process
	taskWaitingTime[0] = 0;            
 
    //calculate waiting time
	for(int i = 1; i < total_Tasks; i++)
    {
		taskWaitingTime[i] = 0;
        
		for( int j = 0; j < i; j++)
			taskWaitingTime[i] = taskWaitingTime[i] + executionTime[j];
 
		total_Time_Calculated = total_Time_Calculated + taskWaitingTime[i];
    }
 
	//average waiting time
	average_WaiitingTime = total_Time_Calculated / total_Tasks;      
    total_Time_Calculated = 0;
 
	cout<<"Aircraft Tasks ordered according to Priority.\n";
	//Printing of Characteristics of the process
    cout<<"\nAirCraft Task Type\t    Burst Time    \tWaiting Time\tTurnaround Time";
	for(int i = 0; i < total_Tasks; i++)
    {
        turnAroundTime[i] = executionTime[i]+ taskWaitingTime[i];     //calculate turnaround time
		total_Time_Calculated = total_Time_Calculated + turnAroundTime[i];
		cout<<"\nP["<< taskPriority[i]<<"]\t\t  "<< executionTime[i]<<"\t\t    "<<taskWaitingTime[i]<<"\t\t\t"<<turnAroundTime[i];
    }
 
	average_TurnaroundTime = total_Time_Calculated / total_Tasks;     //average turnaround time
   
	cout<<"\nPrinting out the Aircraft system characteristics.\n";

	//system characteristics for the priority scheduling
	cout<<"\n\nAverage Waiting Time="<<average_WaiitingTime<<endl;
	cout<<"\nAverage Turnaround Time="<<average_TurnaroundTime<<endl;
	  
  }
}; //end of class

int main()
{	
	AircraftScheduler AC_Scheduler;

	//aircraft monitoring system Task
	AircraftMonitoringSystem acTasksToExecute;
	
	//activate the scheduler
	acTasksToExecute.system_Status = true;
	
	//aircraft monitoring system objects
	AircraftMonitoringSystem engineFuelSensor;
	AircraftMonitoringSystem engineTemperatureSensor;
	AircraftMonitoringSystem smokeDetectorSensor1;
	AircraftMonitoringSystem smokeDetectorSensor2; 
	AircraftMonitoringSystem fuelSensor;

	//============================================================//
	cout<<"\nPrinting the Dails Screen Options:\n";
	cout<<"++++++++++ [DIALS SCREEN OPTION] ++++++++++\n";

	engineFuelSensor.ShowDialsofACSystem("Engine Fuel Sensor");
	engineFuelSensor.ShowDialsofACSystem("Engine Temperature Sensor");
	smokeDetectorSensor1.ShowDialsofACSystem("Smoke Detector 1 Sensor");
	smokeDetectorSensor2.ShowDialsofACSystem("Smoke Detector 2 Sensor");
	fuelSensor.ShowDialsofACSystem("Fuel Sensor");

	//=============================================================//
	//Defining the execution times, deadline, and period of the Aircraft System Tasks 
	chrono::milliseconds execTime_1(50);
	chrono::milliseconds execTime_2(150);
	chrono::milliseconds execTime_3(250);
	chrono::milliseconds execTime_4(350);
	chrono::milliseconds execTime_5(450);

	//defining the deadline of the Aircraft Tasks
	chrono::milliseconds task_deadline_option_one(500);
	chrono::milliseconds task_deadline_option_two(550);
	chrono::milliseconds task_deadline_option_three(600);
	chrono::milliseconds task_deadline_option_four(650);
	chrono::milliseconds task_deadline_option_five(700);
	
	//defining the period of the task
	chrono::milliseconds ac_Task_Period(800);

	//==============================================================//
	//defining the aircraft tasks
	AirCraftTasks SmokeGenerator_One("Smoke Generator 1 AC_Task", ac_Task_Period, execTime_1,
		task_deadline_option_one, "Task Initiated");
	AirCraftTasks SmokeGenerator_Two("Smoke Generator 2 AC_Task", ac_Task_Period, execTime_2,
		task_deadline_option_two, "Task Initiated");

	AirCraftTasks EngineTemperatureSensor("Engine Temperature Sensor", ac_Task_Period, execTime_3,
		task_deadline_option_three, "Task Initiated");

	AirCraftTasks EnginePressureSensor("Engine Pressure Sensor", ac_Task_Period, execTime_4,
		task_deadline_option_four, "Task Initiation");
	AirCraftTasks EngineFuelSensor("Engine Fuel Sensor", ac_Task_Period, execTime_5,
		task_deadline_option_five, "Task Initiated");

	//=============================================================//
	long incrementer = 1;

	//Object to execute all of the aircraft tasks in the system
	AirCraftTasks execute_AC_Tasks;
	
	//add the tasks to the queue of the AirCraft which have to execute: 5 tasks
	execute_AC_Tasks.executing_Tasks.push(1);
	execute_AC_Tasks.executing_Tasks.push(2);
	execute_AC_Tasks.executing_Tasks.push(3);
	execute_AC_Tasks.executing_Tasks.push(4);
	execute_AC_Tasks.executing_Tasks.push(5);

	//define the start time for the Aircraft Task: Engine Fuel Sensor and execute it
	chrono::system_clock::time_point startTime_One = chrono::system_clock::now();
	time_t TimeNow_1 = std::chrono::system_clock::to_time_t(startTime_One + chrono::minutes(incrementer++));
	EngineFuelSensor.execute_AirCraftTasks(startTime_One);
	cout << "\nThe start time of the Engine Fuel Sensor Task is: " << std::put_time(std::localtime(&TimeNow_1), "%c") << '\n';
	execute_AC_Tasks.executed_Tasks.push(1);
	_sleep(3000);
	cout<<"==========================================================";


	//define the start time for the Aircraft Task: Engine Pressure Sensor and execute it
	chrono::system_clock::time_point startTime_Two = chrono::system_clock::now();
	time_t TimeNow_2 = std::chrono::system_clock::to_time_t(startTime_Two + chrono::minutes(incrementer++));
	EnginePressureSensor.execute_AirCraftTasks(startTime_Two);
	cout << "\nThe start time of the Engine Pressure Sensor Task is: " << std::put_time(std::localtime(&TimeNow_2), "%c") << '\n';
	execute_AC_Tasks.executed_Tasks.push(2);
	_sleep(4000);
	cout<<"==========================================================";

	//define the start time for the Aircraft Task: Smoke Generator 1 and execute it
	chrono::system_clock::time_point startTime_Three = chrono::system_clock::now();
	time_t TimeNow_3 = std::chrono::system_clock::to_time_t(startTime_Three + chrono::minutes(incrementer++));
	SmokeGenerator_One.execute_AirCraftTasks(startTime_Three);
	cout << "\nThe start time of the Smoke Generator 1 Task is: " << std::put_time(std::localtime(&TimeNow_3), "%c") << '\n';
	execute_AC_Tasks.executed_Tasks.push(3);
	_sleep(5000);
	cout<<"==========================================================";


	//define the start time for the Aircraft Task: Smoke Generator 2 and execute it
	chrono::system_clock::time_point startTime_Four = chrono::system_clock::now();
	time_t TimeNow_4 = std::chrono::system_clock::to_time_t(startTime_Four + chrono::minutes(incrementer++));
	SmokeGenerator_Two.execute_AirCraftTasks(startTime_Four);
	cout << "\nThe start time of the Smoke Generator 2 Sensor Task is: " << std::put_time(std::localtime(&TimeNow_4), "%c") << '\n';
	execute_AC_Tasks.executed_Tasks.push(4);
	_sleep(6000);
	cout<<"==========================================================";

	//define the start time for the Aircraft Task: EngineTemperatureSensor and execute it
	chrono::system_clock::time_point startTime_Five = chrono::system_clock::now();
	time_t TimeNow_5 = std::chrono::system_clock::to_time_t(startTime_Five + chrono::minutes(incrementer++));
	EngineTemperatureSensor.execute_AirCraftTasks(startTime_Five);
	cout << "\nThe start time of the Engine Temperature Sensor Task is: " << std::put_time(std::localtime(&TimeNow_5), "%c") << '\n';
	execute_AC_Tasks.executed_Tasks.push(5);
	_sleep(7000);
	cout<<"==========================================================";	
	//============================================================//
			
	//define the IH tasks in the system
	AC_Scheduler.InteruptHandlerTasks(&SmokeGenerator_One);
	AC_Scheduler.InteruptHandlerTasks(&SmokeGenerator_Two);


	//initiate the scheduler
	AC_Scheduler.initiateScheduler();
	//start the scheduler tasks
	AC_Scheduler.Scheduler_Execution_Tasks();
	
	cout<<"==========================================================";
	//===========================================================//
	cout<<"\nPrinting the Initial state of the AirCraft Tasks: \n";

	cout<<"\nSmokeGenerator_One Task Status:\n"<<SmokeGenerator_One.warning_Display_Message<<"\n";
	cout<<"\nSmokeGenerator_Two Task Status:\n"<<SmokeGenerator_Two.warning_Display_Message<<"\n";
	cout<<"\nEngineTemperatureSensor Task Status:\n"<<EngineTemperatureSensor.warning_Display_Message<<"\n";
	cout<<"\nEnginePressureSensor Task Status:\n"<<EnginePressureSensor.warning_Display_Message<<"\n";
	cout<<"\nEnginePressureSensor Task Status:\n"<<EngineFuelSensor.warning_Display_Message<<"\n";
	//==========================================================//

	cout<<"==========================================================";
	cout<<"\nStarting to execute a Priority Based Scheduling Case of the Aircraft Monitoring System.\n";
	acTasksToExecute.executePriorityScheduling();	
	//=========================================================//
	//update the state of the tasks

	SmokeGenerator_One.warning_Display_Message = "Warning";
	SmokeGenerator_Two.warning_Display_Message = "Shutdown";
	EngineTemperatureSensor.warning_Display_Message = "Initiated";
	EnginePressureSensor.warning_Display_Message = "Normal";

	//========================================================//
	cout<<"==========================================================";
	cout<<"\nDisplay the Current State of the Tasks in the Magnetic Tape:\n";
	
	cout<<"\nSmokeGenerator_One Task Status: "<<SmokeGenerator_One.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_3), "%c") << '\n';"\n";
	_sleep(5000);
	cout<<"\nSmokeGenerator_Two Task Status: "<<SmokeGenerator_Two.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_4), "%c") << '\n';"\n";
	_sleep(6000);
	cout<<"\nEngineTemperatureSensor Task Status:"<<EngineTemperatureSensor.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_5), "%c") << '\n';"\n";
	_sleep(7000);
	cout<<"\nEnginePressureSensor Task Status: "<<EnginePressureSensor.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_2), "%c") << '\n';"\n";
	_sleep(8000);
	cout<<"\nEngineFuelSensor Task Status: "<<EngineFuelSensor.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_1), "%c") << '\n';"\n";
	//========================================================//
	
	cout<<"==========================================================";
	cout<<"\nStarting to execute 6 iterations Priority Based Case of the Aircraft Monitoring System.\n";
	
	for (int i = 0; i<6 ;)
	{
		acTasksToExecute.executePriorityScheduling();
		
		if(i==3)
		{
			//Update status of tasks 
			SmokeGenerator_One.warning_Display_Message = "Warning";
			SmokeGenerator_Two.warning_Display_Message = "Initiated";
			EngineTemperatureSensor.warning_Display_Message = "Warning";
			EngineFuelSensor.warning_Display_Message = "Warning";
			EnginePressureSensor.warning_Display_Message = "Warning";	

			//print new status		
			cout<<"\nSmokeGenerator_One Task Status: "<<SmokeGenerator_One.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_3), "%c") << '\n';"\n";
			cout<<"\nSmokeGenerator_Two Task Status: "<<SmokeGenerator_Two.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_4), "%c") << '\n';"\n";
			cout<<"\nEngineTemperatureSensor Task Status: "<<EngineTemperatureSensor.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_5), "%c") << '\n';"\n";
			cout<<"\nEnginePressureSensor Task Status: "<<EnginePressureSensor.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_2), "%c") << '\n';"\n";
	        cout<<"\nEngineFuelSensor Task Status: "<<EngineFuelSensor.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_1), "%c") << '\n';"\n";
	
		}
		if(i==5)
		{
			//Update status of tasks 
			SmokeGenerator_One.warning_Display_Message = "Shutdown";
			SmokeGenerator_Two.warning_Display_Message = "Shutdown";
			EngineTemperatureSensor.warning_Display_Message = "Shutdown";
			EngineFuelSensor.warning_Display_Message = "Shutdown";
			EnginePressureSensor.warning_Display_Message = "Shutdown";	

			//print new status		
			cout<<"\nSmokeGenerator_One Task Status: "<<SmokeGenerator_One.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_3), "%c") << '\n';"\n";
			cout<<"\nSmokeGenerator_Two Task Status: "<<SmokeGenerator_Two.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_4), "%c") << '\n';"\n";
			cout<<"\nEngineTemperatureSensor Task Status: "<<EngineTemperatureSensor.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_5), "%c") << '\n';"\n";
			cout<<"\nEnginePressureSensor Task Status: "<<EnginePressureSensor.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_2), "%c") << '\n';"\n";
	        cout<<"\nEngineFuelSensor Task Status: "<<EngineFuelSensor.warning_Display_Message <<" : "<<put_time(std::localtime(&TimeNow_1), "%c") << '\n';"\n";
	
		}
		else 
		{
			++i;
		}
	}
	//=======================================================//
	system("pause");
}
