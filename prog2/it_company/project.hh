/* Class Project
 * ----------
 * COMP.CS.110 FALL 2023
 * ----------
 * Class for describing a project in an IT company.
 *
 * Note: Students need to implement almost all of this class by themselves.
 * */
#ifndef PROJECT_HH
#define PROJECT_HH

#include "employee.hh"
#include "date.hh"
#include "utils.hh"
#include <string>

const std::string NOT_QUALIFIED = "Not qualified any more: ";

class Project
{
public:
    /**
     * @brief Project constructor
     * @param : id
     * @param : start (given as string ddmmyyyy)
     */
    Project(const std::string& id, const std::string& start);

    /**
     * @brief Project constructor
     * @param : id
     * @param : start (given as Date object)
     */
    Project(const std::string& id, const Date& start);

    /**
     * @brief Project destructor
     */
    ~Project();

    // More public methods

    // Return the project ID
    std::string getId() const;

    // Checks if the project is closed
    bool isClosed() const;

    // Marks the project as closed.
    void closeProject();

    // Gets the start date of the project
    Date getStartDate() const;

    // Gets the end date of the project
    Date getEndDate() const;

    // Sets the end date of the project
    void setEndDate(const Date& end);

    // Checks if a specific skill requirement is present in the project
    bool hasRequirement(const std::string& requirement) const;

    // Adds a new skill requirement to the project
    void addRequirement(const std::string& requirement);

    // Retrieves the set of skill requirements for the project
    const std::set<std::string>& getRequirements() const;

    // Checks if the project has a specific employee by their ID
    bool hasEmployee(const std::string& employee_id) const;

    // Assigns an employee to the project
    void assignEmployee(const std::string& employee_id);

    // Retrieves the set of staff members assigned to the project
    const std::set<std::string>& getStaff() const;

    // Sets the staff members for the project based on the provided set of employee IDs
    void setStaff (const std::set<std::string>& assigned_employees);




private:
    /**
     * @brief obvious attributes
     */
    std::string id_; // Can be a name or any other identifier
    Date start_;
    Date end_;

    // More attributes and private methods


    bool is_closed_; // Flag indicating whether the project is closed or not
    std::set<std::string> requirements_; //Set of skill requirements for the project
    std::set<std::string> assigned_employees_;  //Set to store the IDs of employees assigned to the project


};

#endif // PROJECT_HH
