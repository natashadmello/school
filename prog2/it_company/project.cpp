#include "project.hh"
#include <iostream>

Project::Project(const std::string& id, const std::string& start):
    id_(id), start_(start)
{
    is_closed_ = false;
}


Project::Project(const std::string& id, const Date &start):
    id_(id), start_(start)
{
    is_closed_ = false;
}

Project::~Project()
{
    //std::cout << "Project " << id_ << " destructed." << std::endl;
}

// Returns the project ID
std::string Project::getId() const
{
    return id_;
}

// Checks if the project is closed
bool Project::isClosed() const
{
    return is_closed_;
}

// Closes the project if it is not already closed
void Project::closeProject()
{
    if (!is_closed_)
    {
        is_closed_ = true;
    }
}

// Gets the start date of the project
Date Project::getStartDate() const
{
    return start_;
}

// Gets the end date of the project
Date Project::getEndDate() const
{
    return end_;
}

// Sets the end date of the project
void Project::setEndDate(const Date& end)
{
    end_ = end;
}

// Checks if a specific skill requirement is present in the project
bool Project::hasRequirement(const std::string& requirement) const
{
    return requirements_.find(requirement) != requirements_.end();
}

// Adds a new skill requirement to the project
void Project::addRequirement(const std::string& requirement)
{
    requirements_.insert(requirement);
}

// Retrieves the set of skill requirements for the project
const std::set<std::string>& Project::getRequirements() const
{
    return requirements_;
}

// Checks if the project has a specific employee by their ID
bool Project::hasEmployee(const std::string& employee_id) const
{
    return assigned_employees_.count(employee_id) > 0;
}

// Assigns an employee to the project
void Project::assignEmployee(const std::string& employee_id)
{
    assigned_employees_.insert(employee_id);
}

// Retrieves the set of staff members assigned to the project
const std::set<std::string>& Project::getStaff() const
{
    return assigned_employees_;
}

// Sets the staff members for the project based on the provided set of employee IDs
void Project::setStaff(const std::set<std::string>& assigned_employees)
{
    assigned_employees_ = assigned_employees;
}


