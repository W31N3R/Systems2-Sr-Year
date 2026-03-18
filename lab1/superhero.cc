#include "Superhero.h"

// Constructor
Superhero::Superhero(const std::string& first, const std::string& last, const std::string& hero) {
    firstName = new std::string(first);
    lastName = new std::string(last);
    heroName = new std::string(hero);
}

// Copy Constructor
Superhero::Superhero(const Superhero& other) {
    firstName = new std::string(*other.firstName);
    lastName = new std::string(*other.lastName);
    heroName = new std::string(*other.heroName);
}

// Destructor
Superhero::~Superhero() {
    delete firstName;
    delete lastName;
    delete heroName;
}

// Assignment Operator
Superhero& Superhero::operator=(const Superhero& other) {
    if (this != &other) {
        delete firstName;
        delete lastName;
        delete heroName;

        firstName = new std::string(*other.firstName);
        lastName = new std::string(*other.lastName);
        heroName = new std::string(*other.heroName);
    }
    return *this;
}

// Comparison Operator
bool Superhero::operator==(const Superhero& other) const {
    return *firstName == *other.firstName && *lastName == *other.lastName;
}

// Less Than Operator
bool Superhero::operator<(const Superhero& other) const {
    if (*lastName == *other.lastName) {
        return *firstName < *other.firstName;
    }
    return *lastName < *other.lastName;
}

// Output Operator
std::ostream& operator<<(std::ostream& os, const Superhero& hero) {
    os << *hero.firstName << " " << *hero.lastName;
    return os;
}

// Getter for heroName
std::string Superhero::getHeroName() const {
    return *heroName;
}