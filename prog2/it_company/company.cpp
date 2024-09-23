#include "company.hh"
#include "utils.hh"
#include <iostream>
#include <set>
#include <algorithm>

Company::Company()
{
}

Company::~Company()
{
    //std::cout << "Company destructor" << std::endl;

    // Deallocate staff
    for( std::map<std::string, Employee*>::iterator
            iter = all_staff_.begin();
            iter != all_staff_.end();
            ++iter )
       {
           delete iter->second;
       }

    // Deallocate projects
    for (std::map<std::string, Project*>::iterator
          iter = projects_.begin();
          iter != projects_.end();
          ++iter)
     {
         delete iter->second;
     }

}

void Company::set_date(Params params)
{
    std::string day = params.at(0);
    std::string month = params.at(1);
    std::string year = params.at(2);
    if( not Utils::is_numeric(day, false) or
        not Utils::is_numeric(month, false) or
        not Utils::is_numeric(year, false) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    Utils::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    Utils::today.print();
    std::cout << std::endl;
}

void Company::advance_date(Params params)
{
    std::string amount = params.at(0);
    if( not Utils::is_numeric(amount, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    Utils::today.advance(stoi(amount));
    std::cout << "New date is ";
    Utils::today.print();
    std::cout << std::endl;
}

void Company::recruit(Params params)
{
    std::string employee_id = params.at(0);
    std::map<std::string, Employee*>::const_iterator
            iter = current_staff_.find(employee_id);

    if( iter != current_staff_.end() )
    {
        std::cout << ALREADY_EXISTS << employee_id << std::endl;
        return;
    }

    iter = all_staff_.find(employee_id);
    if( iter != all_staff_.end() )
    {
        current_staff_.insert(*iter);
        std::cout << EMPLOYEE_RECRUITED << std::endl;
        return;
    }

    Employee* new_employee = new Employee(employee_id);
    all_staff_.insert({employee_id, new_employee});
    current_staff_.insert({employee_id, new_employee});
    std::cout << EMPLOYEE_RECRUITED << std::endl;
}

void Company::leave(Params params)
{
    std::string employee_id = params.at(0);
    std::map<std::string, Employee*>::const_iterator
            iter = current_staff_.find(employee_id);
    if( iter == current_staff_.end() )
    {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    current_staff_.erase(iter); // Employee still stays in all_staff_
    std::cout << EMPLOYEE_LEFT << std::endl;
}

void Company::add_skill(Params params)
{
    std::string employee_id = params.at(0);
    std::string skill_name = params.at(1);

    std::map<std::string, Employee*>::const_iterator
            staff_iter = current_staff_.find(employee_id);
    if( staff_iter == current_staff_.end() )
    {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    staff_iter->second->add_skill(skill_name);
    std::cout << SKILL_ADDED << employee_id << std::endl;
}

void Company::print_current_staff(Params)
{
    if( current_staff_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }

    for( auto employee : current_staff_ )
    {
        employee.second->print_id("");
        std::cout << std::endl;
    }
}


// Creates a new project for the company.
void Company::create_project(Params params)
{
    // Checks for existing projects with the same ID.
    std::string id = params.at(0);
    if(projects_.find(id) != projects_.end())
    {
        std::cout << ALREADY_EXISTS << params.at(0) << std::endl;
        return;
    }
    // If no duplicate is found, initializes a new project
    Project* p = new Project (params.at(0), Utils::today);

    // Adds it to the project list.
    projects_.insert({id,p});

    //Prints Success message
     std::cout << PROJECT_CREATED << std::endl;
}


// Function to close the project for the company
void Company::close_project(Params params)
{
    std::string id = params.at(0);

    // Checks if the project with the specified ID exists.
    auto it = projects_.find(id);
    if (it != projects_.end())
    {
        // Check if the project is already closed
        if (!it->second->isClosed())
        {
            // Close the project by setting the end date to the current date
            it->second->closeProject(); // Call closeProject to set end date
            it->second->setEndDate(Utils::today);


            std::cout << PROJECT_CLOSED << std::endl;
        }
        else
        {
            // Project is already closed
            std::cout << PROJECT_CLOSED << std::endl;
        }
    }
    else
    {
        // Project not found
        std::cout << CANT_FIND << id << std::endl;
    }
}


// Prints the list of projects for the company
void Company::print_projects(Params)
{
    // If there are no projects, prints "None"
    if (projects_.empty())
    {
        std::cout << "None" << std::endl;
    }
    else
    {
        // Else create a vector to store projects for sorting
        std::vector<Project*> sortedProjects;

        // Copy projects to the vector
        for (const auto& project : projects_)
        {
            sortedProjects.push_back(project.second);
        }

        // Sort projects based on start date
        std::sort(sortedProjects.begin(), sortedProjects.end(),
                  [](const Project* a, const Project* b) {
                      return a->getStartDate() < b->getStartDate();
                  });

        // Print sorted projects
        for (const auto& project : sortedProjects)
        {
            std::cout << project->getId() << " : ";
            project->getStartDate().print();

            if (!project->getEndDate().is_default())
            {
                std::cout << " - ";
                project->getEndDate().print();
            }

            else
            {
                std::cout << " -";
            }

            std::cout << std::endl;
        }
    }
}


// Adds a new requirement to a specified project
void Company::add_requirement(Params params)
{
    std::string project_id = params.at(0);
    std::string requirement = params.at(1);

    // Checks if the project with the given ID exists
    auto project_iter = projects_.find(project_id);

    if (project_iter != projects_.end())
    {
        Project* project = project_iter->second;
        // Check if the project is closed
        if (!project->isClosed())
        {
            // Check if the requirement already exists for the project
            if (project->hasRequirement(requirement))
            {
                // Requirement already exists, print a message
                std::cout << REQUIREMENT_ADDED << project_id << std::endl;
            }
            else
            {
                // Add the requirement to the project
                project->addRequirement(requirement);

                // Remove unqualified employees from the project
                removeEmployee(params);
                std::cout << REQUIREMENT_ADDED << project_id << std::endl;
            }
        }
        else
        {
            // Project is closed, print an error
            std::cout << CANT_FIND << project_id << std::endl;
        }
    }
    else
    {
        // Project not found, print an error
        std::cout << CANT_FIND << project_id << std::endl;
    }
}


// Removes unqualified employees from a specified project based on the project's requirements.
void Company::removeEmployee (Params params)
{
    std::string project_id = params.at(0);
    std::string requirement = params.at(1);

    // Find the specified project
    auto project_iter = projects_.find(project_id);
    Project* project = project_iter->second;

    // Get the current set of assigned employees for the project
    auto assigned_employees = project->getStaff();
    std::set<std::string> erasedEmployee;

     // Iterate through assigned employees, checking for required skills
    for (auto employee_id : assigned_employees)
            {
                auto employee_iter = current_staff_.find(employee_id);
                Employee* employee = employee_iter->second;

                // Check if the employee lacks any of the required skills
                bool lacksSkills = true;
                for (const std::string& req : project->getRequirements())
                {
                    if (employee->has_skill(req))
                    {
                        lacksSkills = false;
                        break;
                    }
                }

                // If employee lacks skills, add their ID to the list of erased employees
                if (lacksSkills)
                {
                    erasedEmployee.insert(employee_id);
                }
            }

    // If there are unqualified employees, print a message and update the project's staff
    if (!erasedEmployee.empty())
    {
        std::cout << NOT_QUALIFIED;

        // Print each unqualified employee's ID
        for (auto it = erasedEmployee.begin(); it != erasedEmployee.end(); ++it)
        {
            std::cout << *it;

            // Print a comma after each element, except the last one
            if (std::next(it) != erasedEmployee.end())
            {
                std::cout << ", ";
            }
            // Remove the unqualified employee from the project's staff
            auto it_assigned = assigned_employees.find(*it);
            if (it_assigned != assigned_employees.end())
            {
                assigned_employees.erase(it_assigned);
            }
        }
        std::cout << std::endl;
    }
    // Update the project's staff with the remaining qualified employees
    project->setStaff(assigned_employees);
}


// Assigns an employee to a specified project
void Company::assign(Params params)
{
    std::string employee_id = params.at(0);
    std::string project_id = params.at(1);

    //Check if employee is in the company
    if (all_staff_.find(employee_id) == all_staff_.end())
    {
            std::cout << CANT_FIND << employee_id << std::endl;
            return;
    }

    // Check if project is valid
    if (projects_.find(project_id) == projects_.end())
    {
        std::cout << CANT_FIND << project_id << std::endl;
        return;
    }

    auto employee = all_staff_.find(employee_id);
    auto project = projects_.find(project_id);

    Employee* employeeptr = employee ->second;
    Project* projectptr = project -> second;

    // Check if the employee has the required skills for the project
    if (!matchemployeetoproject (params))
    {
        std::cout << CANT_ASSIGN  << employee_id << std::endl;
        return;
    }

    // Check if the employee is already assigned to the project
    if (projectptr->hasEmployee(employee_id))
    {
        std::cout << CANT_ASSIGN << employee_id << std::endl;
        return;
    }

    // Check if the project is closed
    if(projectptr->isClosed())
    {
        std::cout << CANT_ASSIGN << employee_id << std::endl;
        return;
    }

    // Assign the project to the employee
    projectptr->assignEmployee(employee_id);
    std::cout << STAFF_ASSIGNED << project_id << std::endl;

    // Assign the employee to the project
    employeeptr->assignProject(project_id);
}

// Checks if an employee has the required skills for a specified project
bool Company::matchemployeetoproject(Params params)
{
    std::string employee_id = params.at(0);
    std::string project_id = params.at(1);

    auto employee = all_staff_.find(employee_id);
    auto project = projects_.find(project_id);

    Employee* employeeptr = employee ->second;
    Project* projectptr = project -> second;

    // Check if the project has no specific skill requirements
    if (projectptr->getRequirements().empty())
    {
        return true;
    }
     // Iterate through the project's skill requirements and check if the employee has each required skill
    for (auto it = projectptr->getRequirements().begin(); it != projectptr->getRequirements().end(); ++it)
    {
      if(employeeptr->has_skill(*it))
      {
          return true;
      }
    }
    // Employee does not meet all skill requirements
    return false;
}

// Prints information about a specified project
void Company::print_project_info(Params params)
{
    // Extract project ID from the params vector
    std::string project_id = params.at(0);
    Project* current_project = nullptr;

    // Find the project directly in the projects_ map
    auto project_iter = projects_.find(project_id);
    if (project_iter != projects_.end())
    {
        current_project = project_iter->second;
    }

    // Check if the project is not found
    if (current_project == nullptr) {
        std::cout << CANT_FIND << project_id << std::endl;
        return;
    }

    // Print project info
    std::cout << current_project->getId() << " : ";
    current_project->getStartDate().print();

    //Add " - " after the start date
    std::cout << " - ";

    //Print end date if the project is closed
    if (current_project->isClosed()) {
        current_project->getEndDate().print();
    }

    std::cout << std::endl;

    // Print requirements
    std::set<std::string> requirements = current_project->getRequirements();
    std::cout << "** Requirements: ";
    if (requirements.empty()) {
        std::cout << "None";
    } else {

        // Sort requirements in alphabetical order
        std::vector<std::string> sortedRequirements(requirements.begin(), requirements.end());
        std::sort(sortedRequirements.begin(), sortedRequirements.end());

        // Print requirements
        auto it = requirements.begin();
        std::cout << *it;
        ++it;
        for (; it != requirements.end(); ++it) {
            std::cout << ", " << *it;
        }
    }
    std::cout << std::endl;

    // Print staff
    const std::set<std::string>& staff = current_project->getStaff();
    std::cout << "** Staff: ";
    if (staff.empty())
    {
        std::cout << "None";
    }
    else
    {
        // Copy the set to a vector for sorting
        std::vector<std::string> sortedStaff(staff.begin(), staff.end());
        std::sort(sortedStaff.begin(), sortedStaff.end());

        // Print staff in alphabetical order
        auto it = sortedStaff.begin();
        while (it != sortedStaff.end())
        {
            std::cout << *it;

            // Move to the next employee
            ++it;

            // Print comma and space if there are more employees
            if (it != sortedStaff.end())
            {
                std::cout << ", ";
            }
        }
    }
    std::cout << std::endl;
}


// Prints information about a specified employee
void Company::print_employee_info(Params params)
{
    std::string employee_id = params.at(0);

    auto employee_it = current_staff_.find(employee_id);

    // Check if the employee is found and active
    if (employee_it != current_staff_.end() && employee_it->second->isActive())
    {
        // Employee found and active
        Employee* employee = employee_it->second;

        // Print skills
        employee->print_skills();

        // Print projects
        const std::vector<std::string>& assigned_projects = employee->getAssignedProjects();

        if (assigned_projects.empty())
        {
            std::cout << "Projects: None" << std::endl;
        }
        else
        {
            std::cout << "Projects:" << std::endl;

            // Print projects in the order of assignment
            for (const auto& project_id : employee->getAssignedProjects())
            {
                Project* project = projects_.at(project_id);

                // Print project info
                std::cout << "** " << project->getId() << " : ";
                project->getStartDate().print();

                std::cout << " - ";

                // Print end date if the project is closed
                if (project->isClosed())
                {
                    project->getEndDate().print();
                }

                std::cout << std::endl;
            }
        }
    }
    else
    {
        // Employee not found or not active
        std::cout << CANT_FIND << employee_id << std::endl;
    }
}

// Prints the IDs of active employees currently assigned to any project
void Company::print_active_staff(Params)
{
    std::set<std::string> activeEmployees;

    // Iterate through all projects
    for (const auto& project : projects_)
    {
        // Check if the project has any assigned employees
        if (!project.second->getStaff().empty())
        {
            // Add assigned employees to the set
            activeEmployees.insert(project.second->getStaff().begin(), project.second->getStaff().end());
        }
    }

    // Print the active employees
    if (activeEmployees.empty())
    {
        std::cout << "None" << std::endl;
    }
    else
    {
        // Print active employees in alphabetical order
        for (const auto& employee : activeEmployees)
        {
            std::cout << employee << std::endl;
        }
    }
}
