# SimLib
Event-driven discrete simulation library written in C++. This library is distributed under MIT license.

## Prerequisites

- compiler with C++14 support (gcc 5 and newer, MSVS 2015 and newer)

## Features

- event-driven simulation
- simple process and event definition
- periodic scheduling using built-in generators (wrappers around standard ones)
- support for more calendars
- fast and secure

## Basic usage
At first, you need to create Simulation object, along with Calendar object and store associated pointers:

```C++
// create simulation object, redirect output to standard output (may be any instance of std::ostream)
auto sim = Simulation::Create(std::cout);
auto cal = Calendar::Create();

sim->Setup(cal);
```

Then you can implement specific objects (events or processes):

```C++
class SampleProcess : public SimProcess
{
    public:
        virtual void Run() override
        {
            std::cout << "Hello!" << std::endl;
            counter++;
            
            if (counter < 10)
            {
                // fire in next 60 time units
                Schedule(GetSimulation()->GetMainCalendar(), 60, true);
            }
        }

    private:
        size_t counter = 0;
};
```

and add them to your simulation:

```C++
auto obj = sim->CreateObject<SampleProcess>();

// at first, fire in next 10 time units
obj->Schedule(cal, 10, true);
```

Then you can run your simulation simply with:
```C++
sim->Run();
```

And everything will be logged to output you selected in simulation initialization.

## Documentation

Documentation is not available at this moment. Sorry.
