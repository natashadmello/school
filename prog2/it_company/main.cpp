/* IT Company
 *
 * Desc:
 *   This program implements a project and employee management system tailored for an
 *   IT company. The system allows for the creation, tracking, and management of
 *   projects and employees within the company. Each project is described by a
 *   unique identifier, start date, and can have various requirements and assigned employees.
 *   The program supports functionalities such as closing projects, setting end dates,
 *   checking project qualifications, and assigning employees to projects.
 *
 * Implemented Functions:
 *   - create_project: Creates a new project with a unique identifier and start date.
 *   - close_project: Closes a project, updating its end date.
 *   - print_projects: Prints information about all active projects.
 *   - add_requirement: Adds a requirement to a specific project.
 *   - assign: Assigns an employee to a project, updating staff details.
 *   - print_project_info: Prints detailed information about a specific project.
 *   - print_employee_info: Prints information about a specific employee.
 *   - print_active_staff: Prints information about all active assigned staff.
 *
 * Program author
 * Name: Natasha Dmello
 *
 * */

#include "cli.hh"
#include "company.hh"

const std::string PROMPT = "IT> ";

int main()
{
    Company* company = new Company();
    Cli cli(company, PROMPT);
    while( cli.exec() ){}

    delete company;
    return EXIT_SUCCESS;
}
