#ifndef STRONGHOLD_H
#define STRONGHOLD_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <memory>
#include <cstring>

 // Forward declarations
class Kingdom;
class Population;
class Army;
class Economy;
class Resource;
class Market;
class Diplomacy;
class Bank;
class RandomEvents;

// Template class for resource management
template <typename T>
class Storage {
private:
    T* items;
    int capacity;
    int count;

public:
    // Constructor
    Storage(int size) : capacity(size), count(0) {
        items = new T[capacity];
    }

    // Destructor
    ~Storage() {
        delete[] items;
    }

    // Add item to storage
    bool add(const T& item) {
        if (count < capacity) {
            items[count++] = item;
            return true;
        }
        return false;
    }

    // Remove item from storage
    bool remove(int index) {
        if (index >= 0 && index < count) {
            for (int i = index; i < count - 1; ++i) {
                items[i] = items[i + 1];
            }
            --count;
            return true;
        }
        return false;
    }

    // Get item at index
    T* get(int index) {
        if (index >= 0 && index < count) {
            return &items[index];
        }
        return nullptr;
    }

    // Get current count
    int getCount() const {
        return count;
    }

    // Get capacity
    int getCapacity() const {
        return capacity;
    }
};

// Base Resource class
class Resource {
protected:
    std::string name;
    int amount;
    double value;

public:
    Resource(const std::string& name, int amount, double value);
    virtual ~Resource();

    std::string getName() const;
    int getAmount() const;
    double getValue() const;

    void setAmount(int newAmount);
    void changeAmount(int delta);
    void setValue(double newValue); // Add this method
    virtual double getTotalValue() const;

    // Virtual method for resource-specific effects
    virtual void applyEffects(Kingdom& kingdom);
};

// Resource types
class Food : public Resource {
public:
    Food(int amount = 0, double value = 1.0);
    void applyEffects(Kingdom& kingdom) override;
};

class Gold : public Resource {
public:
    Gold(int amount = 0, double value = 1.0);
    void applyEffects(Kingdom& kingdom) override;
};

class Wood : public Resource {
public:
    Wood(int amount = 0, double value = 1.0);
    void applyEffects(Kingdom& kingdom) override;
};

class Stone : public Resource {
public:
    Stone(int amount = 0, double value = 1.0);
    void applyEffects(Kingdom& kingdom) override;
};

class Iron : public Resource {
public:
    Iron(int amount = 0, double value = 1.0);
    void applyEffects(Kingdom& kingdom) override;
};

// Base Leader class
class Leader {
protected:
    std::string name;
    int charisma;
    int intelligence;
    int strength;

public:
    Leader(const std::string& name, int charisma, int intelligence, int strength);
    virtual ~Leader();

    std::string getName() const;
    int getCharisma() const;
    int getIntelligence() const;
    int getStrength() const;

    void setName(const std::string& newName);

    // Pure virtual method to represent leader's unique action
    virtual void specialAction(Kingdom& kingdom) = 0;

    // Virtual method for leader-specific effects
    virtual void applyEffects(Kingdom& kingdom);
};

// Leader types
class King : public Leader {
private:
    int royalBloodline;
    int yearsInPower;

public:
    King(const std::string& name, int charisma, int intelligence, int strength, int royalBloodline);
    ~King();

    int getRoyalBloodline() const;
    int getYearsInPower() const;
    void incrementYearsInPower();

    void specialAction(Kingdom& kingdom) override;
    void applyEffects(Kingdom& kingdom) override;
};

class Commander : public Leader {
private:
    int tacticalSkill;
    int loyalty;

public:
    Commander(const std::string& name, int charisma, int intelligence, int strength, int tacticalSkill);
    ~Commander();

    int getTacticalSkill() const;
    int getLoyalty() const;
    void setLoyalty(int newLoyalty);

    void specialAction(Kingdom& kingdom) override;
    void applyEffects(Kingdom& kingdom) override;
};

class GuildLeader : public Leader {
private:
    std::string guildType;
    int businessAcumen;

public:
    GuildLeader(const std::string& name, int charisma, int intelligence, int strength,
        const std::string& guildType, int businessAcumen);
    ~GuildLeader();

    std::string getGuildType() const;
    int getBusinessAcumen() const;

    void specialAction(Kingdom& kingdom) override;
    void applyEffects(Kingdom& kingdom) override;
};

// Population class - manages different population groups
class Population {
private:
    int peasants;
    int merchants;
    int nobles;
    double growthRate;
    double happiness;

public:
    Population(int initialPeasants = 100, int initialMerchants = 20, int initialNobles = 5);
    ~Population();

    int getPeasants() const;
    int getMerchants() const;
    int getNobles() const;
    int getTotal() const;
    double getGrowthRate() const;
    double getHappiness() const;

    void setPeasants(int count);
    void setMerchants(int count);
    void setNobles(int count);
    void setGrowthRate(double rate);
    void setHappiness(double value);

    void updatePopulation(const Economy& economy, const Army& army);
    void calculateHappiness(const Economy& economy, const Army& army);
    bool checkRebellion() const;
};

// Army class - manages military forces
class Army {
private:
    int infantry;
    int cavalry;
    int archers;
    double morale;
    int trainingLevel;
    bool isAtWar;

public:
    Army(int initialInfantry = 50, int initialCavalry = 10, int initialArchers = 20);
    ~Army();

    int getInfantry() const;
    int getCavalry() const;
    int getArchers() const;
    int getTotal() const;
    double getMorale() const;
    int getTrainingLevel() const;
    bool getWarStatus() const;

    void setInfantry(int count);
    void setCavalry(int count);
    void setArchers(int count);
    void setMorale(double value);
    void setTrainingLevel(int level);
    void setWarStatus(bool status);

    void trainArmy();
    int calculateStrength() const;
    void updateMorale(const Economy& economy, const Population& population);
    int calculateDesertion();
    bool checkRebellion(const Population& population) const;
};

// Economy class - manages taxes and finances
class Economy {
private:
    double peasantTaxRate;
    double merchantTaxRate;
    double nobleTaxRate;
    double inflation;
    int treasuryGold;
    int debt;

public:
    Economy(double initialPeasantTaxRate = 0.10, double initialMerchantTaxRate = 0.15,
        double initialNobleTaxRate = 0.20);
    ~Economy();

    double getPeasantTaxRate() const;
    double getMerchantTaxRate() const;
    double getNobleTaxRate() const;
    double getInflation() const;
    int getTreasuryGold() const;
    int getDebt() const;

    void setPeasantTaxRate(double rate);
    void setMerchantTaxRate(double rate);
    void setNobleTaxRate(double rate);
    void setInflation(double value);
    void setTreasuryGold(int amount);
    void setDebt(int amount);

    int collectTaxes(const Population& population);
    void updateEconomy(const Population& population, const Army& army);
    double calculateUnrest(const Population& population) const;
    bool checkRiots(const Population& population) const;
};

// Market class - manages trading and resources
class Market {
private:
    std::shared_ptr<Food> food;
    std::shared_ptr<Gold> gold;
    std::shared_ptr<Wood> wood;
    std::shared_ptr<Stone> stone;
    std::shared_ptr<Iron> iron;
    double priceFluctuation;

public:
    Market();
    ~Market();

    std::shared_ptr<Food> getFood() const;
    std::shared_ptr<Gold> getGold() const;
    std::shared_ptr<Wood> getWood() const;
    std::shared_ptr<Stone> getStone() const;
    std::shared_ptr<Iron> getIron() const;

    void updatePrices(const Economy& economy);
    bool buyResource(const std::string& resourceType, int amount, Economy& economy);
    bool sellResource(const std::string& resourceType, int amount, Economy& economy);
    void produceResources(const Population& population);
    void consumeResources(const Population& population, const Army& army);
};

// Diplomacy class - manages relations with other kingdoms
class Diplomacy {
private:
    struct Kingdom {
        std::string name;
        int relationLevel;
        bool isAlly;
        bool atWar;
        int strength;
    };

    Kingdom* foreignKingdoms;
    int kingdomCount;
    int maxKingdoms;

public:
    Diplomacy(int maxForeignKingdoms = 5);
    ~Diplomacy();

    void addKingdom(const std::string& name, int strength);
    bool improveRelations(const std::string& kingdomName, Economy& economy);
    bool declareWar(const std::string& kingdomName, Army& army);
    bool signPeace(const std::string& kingdomName, Economy& economy);
    bool formAlliance(const std::string& kingdomName);
    bool establishTrade(const std::string& kingdomName, Market& market, Economy& economy);
    void updateDiplomacy(const Army& army, const Economy& economy);
    void listKingdoms() const;
    int getRelationLevel(const std::string& kingdomName) const;
    int getKingdomCount() const {
        return kingdomCount;
    }
    const Kingdom* getForeignKingdoms() const {
        return foreignKingdoms;
    }
    Kingdom* getForeignKingdomsMutable() {
        return foreignKingdoms;
    }
};

// Bank class - manages loans and financial services
class Bank {
private:
    double interestRate;
    int maxLoanAmount;
    int currentLoans;
    int corruptionLevel;

public:
    Bank(double initialInterestRate = 0.05, int initialMaxLoan = 1000);
    ~Bank();

    double getInterestRate() const;
    int getMaxLoanAmount() const;
    int getCurrentLoans() const;
    int getCorruptionLevel() const;

    void setInterestRate(double rate);
    void setMaxLoanAmount(int amount);
    void setCorruptionLevel(int level);

    bool takeLoan(int amount, Economy& economy);
    bool repayLoan(int amount, Economy& economy);
    void updateInterest(Economy& economy);
    void attemptCorruption(Economy& economy, Population& population);
};

// RandomEvents class - manages unpredictable game events
class RandomEvents {
private:
    int eventChance;
    time_t lastEventTime;

public:
    RandomEvents(int chance = 15);
    ~RandomEvents();

    enum EventType {
        PLAGUE,
        GOOD_HARVEST,
        DROUGHT,
        FOREIGN_INVASION,
        REBELLION,
        ASSASSINATION,
        DISCOVERY,
        FESTIVAL,
        FIRE,
        EARTHQUAKE
    };

    bool checkForEvent();
    EventType generateEvent() const;
    void applyEvent(EventType event, Kingdom& kingdom);
    void describePlagueEvent(Kingdom& kingdom);
    void describeGoodHarvestEvent(Kingdom& kingdom);
    void describeDroughtEvent(Kingdom& kingdom);
    void describeForeignInvasionEvent(Kingdom& kingdom);
    void describeRebellionEvent(Kingdom& kingdom);
    void describeAssassinationEvent(Kingdom& kingdom);
    void describeDiscoveryEvent(Kingdom& kingdom);
    void describeFestivalEvent(Kingdom& kingdom);
    void describeFireEvent(Kingdom& kingdom);
    void describeEarthquakeEvent(Kingdom& kingdom);
};

// Kingdom class - the main game class that combines all other systems
class Kingdom {
private:
    std::string name;
    std::unique_ptr<Population> population;
    std::unique_ptr<Army> army;
    std::unique_ptr<Economy> economy;
    std::unique_ptr<Market> market;
    std::unique_ptr<Diplomacy> diplomacy;
    std::unique_ptr<Bank> bank;
    std::unique_ptr<RandomEvents> events;
    std::unique_ptr<Leader> ruler;
    int gameYear;
    int score;

public:
    Kingdom(const std::string& kingdomName);
    ~Kingdom();

    // Getters for component access
    std::string getName() const;
    Population* getPopulation() const;
    Army* getArmy() const;
    Economy* getEconomy() const;
    Market* getMarket() const;
    Diplomacy* getDiplomacy() const;
    Bank* getBank() const;
    RandomEvents* getEvents() const;
    Leader* getRuler() const;
    int getGameYear() const;
    int getScore() const;

    // Setters
    void setName(const std::string& newName);
    void setRuler(std::unique_ptr<Leader> newRuler);
    void setGameYear(int year);
    void setScore(int newScore);

    // Game mechanics
    void advanceYear();
    void calculateScore();
    bool isGameOver() const;
    void displayStatus() const;

    // Save/Load game
    bool saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename);

    // Event handling
    void handleEvent(RandomEvents::EventType event);

    // Elections
    void holdElections();
};

// Function prototypes for main.cpp
void displayMainMenu();
bool processMenuChoice(int choice, Kingdom& kingdom);
bool validateIntInput(int& value, const std::string& prompt, int min, int max);
bool validateDoubleInput(double& value, const std::string& prompt, double min, double max);
bool validateStringInput(std::string& value, const std::string& prompt, int minLength, int maxLength);
void displayResourceManagementMenu(Kingdom& kingdom);
void displayArmyMenu(Kingdom& kingdom);
void displayEconomyMenu(Kingdom& kingdom);
void displayDiplomacyMenu(Kingdom& kingdom);
void displayBankMenu(Kingdom& kingdom);
void clearScreen();
void pauseScreen();

#endif // STRONGHOLD_H