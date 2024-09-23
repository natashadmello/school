#include "employee.hh"
#include <iostream>
#include <map>

Employee::Employee()
{
    is_active_= true;
}

Employee::Employee(const std::string &id):
    id_(id)
{
    date_of_birth_ = Date();
    is_active_= true;
}

Employee::Employee(const std::string& id, const std::string& date_of_birth):
    id_(id), date_of_birth_(date_of_birth)
{
    is_active_= true;
}

Employee::~Employee()
{
    //std::cout << "Employee " << id_ << " destructed." << std::endl;
}

std::string Employee::get_id() const
{
    return id_;
}

void Employee::add_skill(const std::string& skill)
{
    skills_.insert(skill);
}

bool Employee::has_skill(const std::string &skill) const
{
    for( std::string skill_item : skills_ )
    {
        if( skill_item == skill )
        {
            return true;
        }
    }
    return false;
}

void Employee::print_id(const std::string& pre_text) const
{
    std::cout << pre_text << id_;
    //date_of_birth_.print();
    //std::cout << std::endl;
}

void Employee::print_skills() const
{
    std::cout << "Skills: ";
    if( skills_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }

    std::set<std::string>::const_iterator iter = skills_.begin();
    std::cout << *iter; // Printing the first one
    ++iter;
    while( iter != skills_.end() )
    {
        std::cout << ", " << *iter; // Printing the rest
        ++iter;
    }
    std::cout << std::endl;
}

bool Employee::operator<(const Employee &rhs) const
{
    return id_ < rhs.id_;
}

// Getter function for retrieving the list of projects assigned to the employee.
const std::vector<std::string> &Employee::getAssignedProjects() const
{
    return assigned_projects_;
}

//Assigns a new project to the employee
void Employee::assignProject(const std::string& project_id)
{
    assigned_projects_.push_back(project_id);
}

//Checks whether the employee is currently active.
// True if the employee is active, false otherwise.
bool Employee::isActive() const
{
    return is_active_;
}

//Sets the active status of the employee.
void Employee::setActive(bool active)
{
    is_active_ = active;
}

