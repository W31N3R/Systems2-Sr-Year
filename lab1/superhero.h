#ifndef SUPERHERO_H
#define SUPERHERO_H

#include <string>
#include <iostream>

class Superhero {
private:
    std::string* firstName;
    std::string* lastName;
    std::string* heroName;

public:
    // Constructor
    Superhero(const std::string& first, const std::string& last, const std::string& hero);

    // Copy Constructor
    Superhero(const Superhero& other);

    // Destructor
    ~Superhero();

    // Assignment Operator
    Superhero& operator=(const Superhero& other);

    // Comparison Operator
    bool operator==(const Superhero& other) const;

    // Less Than Operator
    bool operator<(const Superhero& other) const;

    // Output Operator
    friend std::ostream& operator<<(std::ostream& os, const Superhero& hero);

    // Getter for heroName
    std::string getHeroName() const;
};

#endif // SUPERHERO_H