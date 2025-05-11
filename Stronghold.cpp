#include "Stronghold.h"

#include <windows.h> // For Sleep()
using namespace std;

// Cross-platform sleep function
void crossPlatformSleep(int seconds) {
#ifdef _WIN32
    Sleep(seconds * 1000); // Sleep takes milliseconds on Windows
#else
    sleep(seconds); // sleep takes seconds on POSIX
#endif
}

// ------------------------
// Resource implementations
// ------------------------

Resource::Resource(const string& name, int amount, double value)
    : name(name), amount(amount), value(value) {
}

Resource::~Resource() {}

string Resource::getName() const {
    return name;
}

int Resource::getAmount() const {
    return amount;
}

double Resource::getValue() const {
    return value;
}

void Resource::setAmount(int newAmount) {
    amount = newAmount < 0 ? 0 : newAmount;
}

void Resource::changeAmount(int delta) {
    amount += delta;
    if (amount < 0) amount = 0;
}

void Resource::setValue(double newValue) {
    value = newValue > 0 ? newValue : 0; // Ensure value is non-negative
}


double Resource::getTotalValue() const {
    return amount * value;
}

void Resource::applyEffects(Kingdom& kingdom) {
    // Base implementation does nothing
}

// Food implementation
Food::Food(int amount, double value)
    : Resource("Food", amount, value) {
}

void Food::applyEffects(Kingdom& kingdom) {
    // Food directly affects population happiness
    double happinessModifier = 0;
    int totalPopulation = kingdom.getPopulation()->getTotal();

    if (totalPopulation > 0) {
        double foodPerPerson = static_cast<double>(amount) / totalPopulation;
        if (foodPerPerson > 1.5) {
            happinessModifier = 0.1; // Plenty of food
        }
        else if (foodPerPerson > 1.0) {
            happinessModifier = 0.05; // Adequate food
        }
        else if (foodPerPerson < 0.5) {
            happinessModifier = -0.2; // Shortage
        }
        else if (foodPerPerson < 0.25) {
            happinessModifier = -0.4; // Severe shortage
        }
    }

    double currentHappiness = kingdom.getPopulation()->getHappiness();
    kingdom.getPopulation()->setHappiness(currentHappiness + happinessModifier);
}

// Gold implementation
Gold::Gold(int amount, double value)
    : Resource("Gold", amount, value) {
}

void Gold::applyEffects(Kingdom& kingdom) {
    // Gold is handled by the Economy class
}

// Wood implementation
Wood::Wood(int amount, double value)
    : Resource("Wood", amount, value) {
}

void Wood::applyEffects(Kingdom& kingdom) {
    // Wood effects might include building capacity, etc.
}

// Stone implementation
Stone::Stone(int amount, double value)
    : Resource("Stone", amount, value) {
}

void Stone::applyEffects(Kingdom& kingdom) {
    // Stone effects might include building capacity, etc.
}

// Iron implementation
Iron::Iron(int amount, double value)
    : Resource("Iron", amount, value) {
}

void Iron::applyEffects(Kingdom& kingdom) {
    // Iron affects army equipment quality
    if (amount > 100) {
        int currentLevel = kingdom.getArmy()->getTrainingLevel();
        kingdom.getArmy()->setTrainingLevel(currentLevel + 1);
    }
}

// ---------------------
// Leader implementations
// ---------------------

Leader::Leader(const string& name, int charisma, int intelligence, int strength)
    : name(name), charisma(charisma), intelligence(intelligence), strength(strength) {
}

Leader::~Leader() {}

string Leader::getName() const {
    return name;
}

int Leader::getCharisma() const {
    return charisma;
}

int Leader::getIntelligence() const {
    return intelligence;
}

int Leader::getStrength() const {
    return strength;
}

void Leader::setName(const string& newName) {
    name = newName;
}

void Leader::applyEffects(Kingdom& kingdom) {
    // Base implementation does nothing
}

// King implementation
King::King(const string& name, int charisma, int intelligence, int strength, int royalBloodline)
    : Leader(name, charisma, intelligence, strength), royalBloodline(royalBloodline), yearsInPower(0) {
}

King::~King() {}

int King::getRoyalBloodline() const {
    return royalBloodline;
}

int King::getYearsInPower() const {
    return yearsInPower;
}

void King::incrementYearsInPower() {
    yearsInPower++;
}

void King::specialAction(Kingdom& kingdom) {
    // King's royal decree: temporarily boost economy or population
    cout << "\nKing " << name << " issues a Royal Decree!" << endl;

    int choice = rand() % 3;
    switch (choice) {
    case 0: // Economic stimulus
        cout << "The decree stimulates the economy, increasing treasury by 10%." << endl;
        kingdom.getEconomy()->setTreasuryGold(
            static_cast<int>(kingdom.getEconomy()->getTreasuryGold() * 1.1)
        );
        break;
    case 1: // Population happiness
        cout << "The decree grants minor tax relief, improving happiness." << endl;
        kingdom.getPopulation()->setHappiness(
            kingdom.getPopulation()->getHappiness() + 0.1
        );
        break;
    case 2: // Military morale
        cout << "The decree honors the military, boosting army morale." << endl;
        kingdom.getArmy()->setMorale(
            kingdom.getArmy()->getMorale() + 0.15
        );
        break;
    }
}

void King::applyEffects(Kingdom& kingdom) {
    // King's passive effects based on stats

    // Charisma affects diplomatic relations
    double diplomacyBonus = charisma * 0.01;
    // Implement diplomacy effects when needed

    // Intelligence affects economy
    double economyBonus = intelligence * 0.01;
    kingdom.getEconomy()->setInflation(
        max(0.01, kingdom.getEconomy()->getInflation() - economyBonus)
    );

    // Strength affects army
    double armyBonus = strength * 0.01;
    kingdom.getArmy()->setMorale(
        min(1.0, kingdom.getArmy()->getMorale() + armyBonus)
    );

    // Royal bloodline affects population loyalty
    double loyaltyBonus = royalBloodline * 0.02;
    kingdom.getPopulation()->setHappiness(
        min(1.0, kingdom.getPopulation()->getHappiness() + loyaltyBonus)
    );
}

// Commander implementation
Commander::Commander(const string& name, int charisma, int intelligence, int strength, int tacticalSkill)
    : Leader(name, charisma, intelligence, strength), tacticalSkill(tacticalSkill), loyalty(50 + rand() % 51) {
}

Commander::~Commander() {}

int Commander::getTacticalSkill() const {
    return tacticalSkill;
}

int Commander::getLoyalty() const {
    return loyalty;
}

void Commander::setLoyalty(int newLoyalty) {
    loyalty = max(0, min(100, newLoyalty));
}

void Commander::specialAction(Kingdom& kingdom) {
    // Commander's special action: military drill or defense improvement
    cout << "\nCommander " << name << " conducts special military operations!" << endl;

    // Training takes time
    cout << "Training troops... ";
    for (int i = 0; i < 3; i++) {
        cout << "." << flush;
        crossPlatformSleep(1);
    }
    cout << " Complete!" << endl;

    int choice = rand() % 2;
    switch (choice) {
    case 0: // Military training
        cout << "The army's training level increases!" << endl;
        kingdom.getArmy()->setTrainingLevel(
            kingdom.getArmy()->getTrainingLevel() + 1 + (tacticalSkill / 20)
        );
        break;
    case 1: // Morale boost
        cout << "Troop morale is significantly improved!" << endl;
        kingdom.getArmy()->setMorale(
            min(1.0, kingdom.getArmy()->getMorale() + 0.2 + (charisma * 0.01))
        );
        break;
    }
}

void Commander::applyEffects(Kingdom& kingdom) {
    // Commander's passive effects

    // Tactical skill affects army strength
    double armyStrengthBonus = tacticalSkill * 0.02;
    double currentMorale = kingdom.getArmy()->getMorale();
    kingdom.getArmy()->setMorale(min(1.0, currentMorale + armyStrengthBonus * 0.1));

    // Loyalty affects chance of rebellion
    if (loyalty < 30 && rand() % 100 < (30 - loyalty)) {
        cout << "\nWARNING: Commander " << name << " is plotting against you!" << endl;
        // Potentially trigger rebellion event
    }
}

// GuildLeader implementation
GuildLeader::GuildLeader(const string& name, int charisma, int intelligence, int strength,
    const string& guildType, int businessAcumen)
    : Leader(name, charisma, intelligence, strength), guildType(guildType), businessAcumen(businessAcumen) {
}

GuildLeader::~GuildLeader() {}

string GuildLeader::getGuildType() const {
    return guildType;
}

int GuildLeader::getBusinessAcumen() const {
    return businessAcumen;
}

void GuildLeader::specialAction(Kingdom& kingdom) {
    // Guild leader's special action: economic boost or trade deals
    cout << "\nGuild Leader " << name << " of the " << guildType << " Guild initiates a special project!" << endl;

    if (guildType == "Merchants") {
        cout << "New trade deals bring increased tax revenue!" << endl;
        kingdom.getEconomy()->setTreasuryGold(
            kingdom.getEconomy()->getTreasuryGold() + 100 + (businessAcumen * 5)
        );
    }
    else if (guildType == "Craftsmen") {
        cout << "Improved crafting techniques boost resource production!" << endl;
        kingdom.getMarket()->getWood()->changeAmount(50 + (businessAcumen * 2));
        kingdom.getMarket()->getIron()->changeAmount(20 + (businessAcumen * 1));
    }
    else if (guildType == "Farmers") {
        cout << "Agricultural innovations increase food stocks!" << endl;
        kingdom.getMarket()->getFood()->changeAmount(100 + (businessAcumen * 5));
    }
}

void GuildLeader::applyEffects(Kingdom& kingdom) {
    // Guild leader's passive effects

    // Business acumen affects economy
    double economyBonus = businessAcumen * 0.02;
    kingdom.getEconomy()->setInflation(
        max(0.01, kingdom.getEconomy()->getInflation() - economyBonus * 0.01)
    );

    // Guild type specific effects
    if (guildType == "Merchants") {
        // Merchants boost trade income
        int merchantCount = kingdom.getPopulation()->getMerchants();
        int bonusGold = (merchantCount * businessAcumen) / 100;
        kingdom.getEconomy()->setTreasuryGold(
            kingdom.getEconomy()->getTreasuryGold() + bonusGold
        );
    }
    else if (guildType == "Craftsmen") {
        // Craftsmen improve resource efficiency
        // Implementation would adjust resource production rates
    }
    else if (guildType == "Farmers") {
        // Farmers improve food production
        kingdom.getMarket()->getFood()->changeAmount((businessAcumen / 10) + 5);
    }
}

// ------------------------
// Population implementation
// ------------------------

Population::Population(int initialPeasants, int initialMerchants, int initialNobles)
    : peasants(initialPeasants), merchants(initialMerchants), nobles(initialNobles),
    growthRate(0.05), happiness(0.5) {
}

Population::~Population() {}

int Population::getPeasants() const {
    return peasants;
}

int Population::getMerchants() const {
    return merchants;
}

int Population::getNobles() const {
    return nobles;
}

int Population::getTotal() const {
    return peasants + merchants + nobles;
}

double Population::getGrowthRate() const {
    return growthRate;
}

double Population::getHappiness() const {
    return happiness;
}

void Population::setPeasants(int count) {
    peasants = max(0, count);
}

void Population::setMerchants(int count) {
    merchants = max(0, count);
}

void Population::setNobles(int count) {
    nobles = max(0, count);
}

void Population::setGrowthRate(double rate) {
    growthRate = max(0.0, min(0.2, rate));
}

void Population::setHappiness(double value) {
    happiness = max(0.0, min(1.0, value));
}

void Population::updatePopulation(const Economy& economy, const Army& army) {
    // Update growth rate based on conditions
    double taxBurden = economy.getPeasantTaxRate() + economy.getMerchantTaxRate() + economy.getNobleTaxRate();
    double foodSecurity = 1.0; // Placeholder, would be calculated from food resources

    // Adjust growth rate based on conditions
    growthRate = 0.05 + (happiness * 0.05) - (taxBurden * 0.1) + (foodSecurity * 0.02);
    growthRate = max(0.01, min(0.2, growthRate)); // Clamp to reasonable range

    // Apply growth to different population groups
    peasants += static_cast<int>(peasants * growthRate);
    merchants += static_cast<int>(merchants * (growthRate * 0.8)); // Merchants grow more slowly
    nobles += static_cast<int>(nobles * (growthRate * 0.5)); // Nobles grow very slowly

    // Potential for social mobility
    if (rand() % 100 < 5) { // 5% chance each year
        int socialMobility = max(1, static_cast<int>(peasants * 0.01));
        peasants -= socialMobility;
        merchants += socialMobility;
    }

    if (rand() % 100 < 2) { // 2% chance each year
        int socialMobility = max(1, static_cast<int>(merchants * 0.01));
        merchants -= socialMobility;
        nobles += socialMobility;
    }
}

void Population::calculateHappiness(const Economy& economy, const Army& army) {
    // Factors affecting happiness
    double taxFactor = 1.0 - ((economy.getPeasantTaxRate() * 2) +
        (economy.getMerchantTaxRate() * 1.5) +
        (economy.getNobleTaxRate() * 0.5));
    double armyPresence = min(1.0, static_cast<double>(army.getTotal()) / static_cast<double>(getTotal()) * 0.5);
    double inflationFactor = 1.0 - (economy.getInflation() * 2.0);

    // Calculate new happiness
    double newHappiness = (happiness * 0.7) + (taxFactor * 0.1) +
        (armyPresence * 0.1) + (inflationFactor * 0.1);

    // Clamp to valid range
    happiness = max(0.0, min(1.0, newHappiness));
}

bool Population::checkRebellion() const {
    // Check if population is going to rebel
    if (happiness < 0.2) {
        // Very unhappy population might rebel
        return (rand() % 100) < ((0.2 - happiness) * 100 * 2);
    }
    return false;
}

// -------------------
// Army implementation
// -------------------

Army::Army(int initialInfantry, int initialCavalry, int initialArchers)
    : infantry(initialInfantry), cavalry(initialCavalry), archers(initialArchers),
    morale(0.7), trainingLevel(1), isAtWar(false) {
}

Army::~Army() {}

int Army::getInfantry() const {
    return infantry;
}

int Army::getCavalry() const {
    return cavalry;
}

int Army::getArchers() const {
    return archers;
}

int Army::getTotal() const {
    return infantry + cavalry + archers;
}

double Army::getMorale() const {
    return morale;
}

int Army::getTrainingLevel() const {
    return trainingLevel;
}

bool Army::getWarStatus() const {
    return isAtWar;
}

void Army::setInfantry(int count) {
    infantry = max(0, count);
}

void Army::setCavalry(int count) {
    cavalry = max(0, count);
}

void Army::setArchers(int count) {
    archers = max(0, count);
}

void Army::setMorale(double value) {
    morale = max(0.0, min(1.0, value));
}

void Army::setTrainingLevel(int level) {
    trainingLevel = max(1, level);
}

void Army::setWarStatus(bool status) {
    isAtWar = status;
}

void Army::trainArmy() {
    // Training the army takes time and resources but improves effectiveness
    cout << "Training army units... ";
    for (int i = 0; i < 3; i++) {
        cout << "." << flush;
        crossPlatformSleep(1);
    }
    cout << " Complete!" << endl;

    // Improve training level
    trainingLevel++;

    // Boost morale
    morale = min(1.0, morale + 0.1);

    cout << "Army training level increased to " << trainingLevel << endl;
    cout << "Morale improved to " << static_cast<int>(morale * 100) << "%" << endl;
}

int Army::calculateStrength() const {
    // Calculate the overall military strength
    int baseStrength = infantry + (cavalry * 3) + (archers * 2);
    double moraleMultiplier = 0.5 + (morale * 0.5); // 0.5 - 1.0
    double trainingMultiplier = 0.8 + (trainingLevel * 0.2); // Starts at 1.0

    return static_cast<int>(baseStrength * moraleMultiplier * trainingMultiplier);
}

void Army::updateMorale(const Economy& economy, const Population& population) {
    // Factors affecting morale
    double payFactor = min(1.0, static_cast<double>(economy.getTreasuryGold()) /
        (getTotal() * 5)); // Can the kingdom pay the troops?
    double populationSupport = population.getHappiness();
    double warEffect = isAtWar ? -0.1 : 0.05; // War decreases morale over time

    // Calculate new morale
    double newMorale = (morale * 0.7) + (payFactor * 0.1) +
        (populationSupport * 0.1) + warEffect;

    // Clamp to valid range
    morale = max(0.1, min(1.0, newMorale));
}

int Army::calculateDesertion() {
    // Calculate how many troops desert based on morale
    if (morale < 0.4) {
        double desertionRate = (0.4 - morale) * 0.5; // Up to 20% desertion for very low morale
        int deserters = static_cast<int>(getTotal() * desertionRate);

        // Distribute desertion across unit types
        int infantryDeserters = min(infantry, static_cast<int>(deserters * 0.6));
        int cavalryDeserters = min(cavalry, static_cast<int>(deserters * 0.2));
        int archerDeserters = min(archers, static_cast<int>(deserters * 0.2));

        infantry -= infantryDeserters;
        cavalry -= cavalryDeserters;
        archers -= archerDeserters;

        return infantryDeserters + cavalryDeserters + archerDeserters;
    }
    return 0;
}

bool Army::checkRebellion(const Population& population) const {
    // Check if the army will rebel against the ruler
    if (morale < 0.2 && population.getHappiness() < 0.3) {
        // Both army and population are very unhappy
        return (rand() % 100) < ((0.2 - morale) * 100 * 3);
    }
    return false;
}

// ----------------------
// Economy implementation
// ----------------------

Economy::Economy(double initialPeasantTaxRate, double initialMerchantTaxRate, double initialNobleTaxRate)
    : peasantTaxRate(initialPeasantTaxRate), merchantTaxRate(initialMerchantTaxRate),
    nobleTaxRate(initialNobleTaxRate), inflation(0.02), treasuryGold(1000), debt(0) {
}

Economy::~Economy() {}

double Economy::getPeasantTaxRate() const {
    return peasantTaxRate;
}

double Economy::getMerchantTaxRate() const {
    return merchantTaxRate;
}

double Economy::getNobleTaxRate() const {
    return nobleTaxRate;
}

double Economy::getInflation() const {
    return inflation;
}

int Economy::getTreasuryGold() const {
    return treasuryGold;
}

int Economy::getDebt() const {
    return debt;
}

void Economy::setPeasantTaxRate(double rate) {
    peasantTaxRate = max(0.0, min(0.5, rate));
}

void Economy::setMerchantTaxRate(double rate) {
    merchantTaxRate = max(0.0, min(0.5, rate));
}

void Economy::setNobleTaxRate(double rate) {
    nobleTaxRate = max(0.0, min(0.5, rate));
}

void Economy::setInflation(double value) {
    inflation = max(0.01, min(0.2, value));
}

void Economy::setTreasuryGold(int amount) {
    treasuryGold = max(0, amount);
}

void Economy::setDebt(int amount) {
    debt = max(0, amount);
}

int Economy::collectTaxes(const Population& population) {
    // Calculate tax revenue from different population groups
    int peasantTax = static_cast<int>(population.getPeasants() * 2 * peasantTaxRate);
    int merchantTax = static_cast<int>(population.getMerchants() * 10 * merchantTaxRate);
    int nobleTax = static_cast<int>(population.getNobles() * 50 * nobleTaxRate);

    int totalTax = peasantTax + merchantTax + nobleTax;
    treasuryGold += totalTax;

    return totalTax;
}

void Economy::updateEconomy(const Population& population, const Army& army) {
    // Update economic factors

    // Army maintenance costs
    int armyCost = army.getTotal() * 2;
    treasuryGold -= min(treasuryGold, armyCost);

    // Bureaucracy costs
    int bureaucracyCost = population.getTotal() / 10;
    treasuryGold -= min(treasuryGold, bureaucracyCost);

    // Update inflation based on economic activity
    double economicActivity = static_cast<double>(population.getTotal()) / 1000.0;
    double treasuryRatio = min(1.0, static_cast<double>(treasuryGold) / 10000.0);

    // Inflation increases with high economic activity and low treasury
    inflation = (inflation * 0.8) + (economicActivity * 0.05) - (treasuryRatio * 0.03);
    inflation = max(0.01, min(0.2, inflation));

    // Apply debt interest
    if (debt > 0) {
        int interest = static_cast<int>(debt * 0.1); // 10% interest
        debt += interest;
    }
}

double Economy::calculateUnrest(const Population& population) const {
    // Calculate economic unrest level
    double taxBurden = (peasantTaxRate + merchantTaxRate + nobleTaxRate) / 3.0;
    double inflationImpact = inflation * 5.0;
    double happinessOffset = population.getHappiness();

    return min(1.0, (taxBurden * 0.5) + (inflationImpact * 0.3) - (happinessOffset * 0.5));
}

bool Economy::checkRiots(const Population& population) const {
    // Check if economic conditions will cause riots
    double unrest = calculateUnrest(population);
    return (unrest > 0.6) && ((rand() % 100) < (unrest * 100));
}

// --------------------
// Market implementation
// --------------------

Market::Market()
    : food(make_shared<Food>(1000)), gold(make_shared<Gold>(500)),
    wood(make_shared<Wood>(500)), stone(make_shared<Stone>(300)),
    iron(make_shared<Iron>(200)), priceFluctuation(0.1) {
}

Market::~Market() {}

shared_ptr<Food> Market::getFood() const {
    return food;
}

shared_ptr<Gold> Market::getGold() const {
    return gold;
}

shared_ptr<Wood> Market::getWood() const {
    return wood;
}

shared_ptr<Stone> Market::getStone() const {
    return stone;
}

shared_ptr<Iron> Market::getIron() const {
    return iron;
}

void Market::updatePrices(const Economy& economy) {
    // Update resource prices based on economy and random fluctuations
    double inflationFactor = 1.0 + economy.getInflation();

    // Apply inflation to base values
    food->setValue(1.0 * inflationFactor * (1.0 + ((rand() % 20) - 10) * 0.01));
    wood->setValue(2.0 * inflationFactor * (1.0 + ((rand() % 20) - 10) * 0.01));
    stone->setValue(3.0 * inflationFactor * (1.0 + ((rand() % 20) - 10) * 0.01));
    iron->setValue(5.0 * inflationFactor * (1.0 + ((rand() % 20) - 10) * 0.01));
}

bool Market::buyResource(const string& resourceType, int amount, Economy& economy) {
    // Buy resources from the market
    int cost = 0;
    shared_ptr<Resource> resource = nullptr;

    if (resourceType == "Food") {
        cost = static_cast<int>(amount * food->getValue());
        resource = food;
    }
    else if (resourceType == "Wood") {
        cost = static_cast<int>(amount * wood->getValue());
        resource = wood;
    }
    else if (resourceType == "Stone") {
        cost = static_cast<int>(amount * stone->getValue());
        resource = stone;
    }
    else if (resourceType == "Iron") {
        cost = static_cast<int>(amount * iron->getValue());
        resource = iron;
    }
    else {
        return false;
    }

    // Check if the kingdom can afford it
    if (economy.getTreasuryGold() >= cost) {
        economy.setTreasuryGold(economy.getTreasuryGold() - cost);
        resource->changeAmount(amount);
        return true;
    }

    return false;
}

bool Market::sellResource(const string& resourceType, int amount, Economy& economy) {
    // Sell resources to the market
    int revenue = 0;
    shared_ptr<Resource> resource = nullptr;

    if (resourceType == "Food") {
        if (food->getAmount() < amount) return false;
        revenue = static_cast<int>(amount * food->getValue() * 0.9); // 10% market fee
        resource = food;
    }
    else if (resourceType == "Wood") {
        if (wood->getAmount() < amount) return false;
        revenue = static_cast<int>(amount * wood->getValue() * 0.9);
        resource = wood;
    }
    else if (resourceType == "Stone") {
        if (stone->getAmount() < amount) return false;
        revenue = static_cast<int>(amount * stone->getValue() * 0.9);
        resource = stone;
    }
    else if (resourceType == "Iron") {
        if (iron->getAmount() < amount) return false;
        revenue = static_cast<int>(amount * iron->getValue() * 0.9);
        resource = iron;
    }
    else {
        return false;
    }

    // Complete the transaction
    resource->changeAmount(-amount);
    economy.setTreasuryGold(economy.getTreasuryGold() + revenue);
    return true;
}

void Market::produceResources(const Population& population) {
    // Calculate resource production based on population
    int peasantProduction = population.getPeasants() / 5;
    int merchantProduction = population.getMerchants() / 2;

    // Food production (mainly from peasants)
    food->changeAmount(peasantProduction * 2);

    // Wood production
    wood->changeAmount(peasantProduction);

    // Stone production
    stone->changeAmount(peasantProduction / 2);

    // Iron production (less common)
    iron->changeAmount(peasantProduction / 4);

    // Gold from merchant activity
    gold->changeAmount(merchantProduction * 2);
}

void Market::consumeResources(const Population& population, const Army& army) {
    // Calculate resource consumption
    int totalPopulation = population.getTotal();
    int totalArmy = army.getTotal();

    // Food consumption
    int foodConsumption = totalPopulation + (totalArmy * 2); // Army eats more
    food->changeAmount(-min(food->getAmount(), foodConsumption));

    // Wood consumption (for heating, building, etc.)
    int woodConsumption = totalPopulation / 10;
    wood->changeAmount(-min(wood->getAmount(), woodConsumption));

    // Iron consumption (for tools, weapons)
    int ironConsumption = totalPopulation / 50 + totalArmy / 20;
    iron->changeAmount(-min(iron->getAmount(), ironConsumption));
}

// ------------------------
// Diplomacy implementation
// ------------------------

Diplomacy::Diplomacy(int maxForeignKingdoms)
    : maxKingdoms(maxForeignKingdoms), kingdomCount(0) {

    foreignKingdoms = new Kingdom[maxKingdoms];

    // Initialize with some default kingdoms
    addKingdom("Northlands", 500 + rand() % 500);
    addKingdom("Eastern Empire", 600 + rand() % 600);
    addKingdom("Southern Realms", 400 + rand() % 400);
}

Diplomacy::~Diplomacy() {
    delete[] foreignKingdoms;
}

void Diplomacy::addKingdom(const string& name, int strength) {
    if (kingdomCount < maxKingdoms) {
        foreignKingdoms[kingdomCount].name = name;
        foreignKingdoms[kingdomCount].relationLevel = 0; // Neutral
        foreignKingdoms[kingdomCount].isAlly = false;
        foreignKingdoms[kingdomCount].atWar = false;
        foreignKingdoms[kingdomCount].strength = strength;
        kingdomCount++;
    }
}

bool Diplomacy::improveRelations(const string& kingdomName, Economy& economy) {
    for (int i = 0; i < kingdomCount; i++) {
        if (foreignKingdoms[i].name == kingdomName) {
            // Lower cost and increase relation impact
            int cost = 20 + (foreignKingdoms[i].relationLevel * 5); // Reduced cost

            if (economy.getTreasuryGold() >= cost) {
                economy.setTreasuryGold(economy.getTreasuryGold() - cost);
                foreignKingdoms[i].relationLevel = min(10, foreignKingdoms[i].relationLevel + 2); // +2 instead of +1
                cout << "Spent " << cost << " gold to improve relations!" << endl;
                return true;
            }
            else {
                cout << "Not enough gold! Need " << cost << " gold." << endl;
                return false;
            }
        }
    }
    cout << "Kingdom '" << kingdomName << "' not found!" << endl;
    return false;
}

bool Diplomacy::declareWar(const string& kingdomName, Army& army) {
    for (int i = 0; i < kingdomCount; i++) {
        if (foreignKingdoms[i].name == kingdomName) {
            if (!foreignKingdoms[i].atWar) {
                foreignKingdoms[i].atWar = true;
                foreignKingdoms[i].isAlly = false;
                foreignKingdoms[i].relationLevel = max(-10, foreignKingdoms[i].relationLevel - 5); // More significant drop
                army.setWarStatus(true);
                cout << "Your army mobilizes for war!" << endl;
                return true;
            }
            cout << "Already at war with " << kingdomName << "!" << endl;
            return false;
        }
    }
    cout << "Kingdom '" << kingdomName << "' not found!" << endl;
    return false;
}

bool Diplomacy::signPeace(const string& kingdomName, Economy& economy) {
    // Find the kingdom
    Army army;
    for (int i = 0; i < kingdomCount; i++) {
        if (foreignKingdoms[i].name == kingdomName) {
            if (foreignKingdoms[i].atWar) {
                // Peace treaties often require reparations
                int cost = 200 + (foreignKingdoms[i].strength / 10);

                if (economy.getTreasuryGold() >= cost) {
                    economy.setTreasuryGold(economy.getTreasuryGold() - cost);
                    foreignKingdoms[i].atWar = false;
                    foreignKingdoms[i].relationLevel = 0; // Reset to neutral

                    // Check if still at war with any kingdom
                    bool stillAtWar = false;
                    for (int j = 0; j < kingdomCount; j++) {
                        if (foreignKingdoms[j].atWar) {
                            stillAtWar = true;
                            break;
                        }
                    }

                    // Update army war status if no longer at war with anyone
                    if (!stillAtWar) {
                        army.setWarStatus(false);
                    }

                    return true;
                }
                else {
                    return false; // Not enough gold
                }
            }
            return false; // Not at war
        }
    }
    return false; // Kingdom not found
}

bool Diplomacy::formAlliance(const string& kingdomName) {
    for (int i = 0; i < kingdomCount; i++) {
        if (foreignKingdoms[i].name == kingdomName) {
            if (!foreignKingdoms[i].atWar && foreignKingdoms[i].relationLevel >= 5) { // Lowered from 7
                foreignKingdoms[i].isAlly = true;
                foreignKingdoms[i].relationLevel = min(10, foreignKingdoms[i].relationLevel + 1); // Bonus relation
                cout << kingdomName << " is now your ally!" << endl;
                return true;
            }
            cout << "Cannot ally! Relations too low (need 5+) or at war." << endl;
            return false;
        }
    }
    cout << "Kingdom '" << kingdomName << "' not found!" << endl;
    return false;
}

bool Diplomacy::establishTrade(const string& kingdomName, Market& market, Economy& economy) {
    for (int i = 0; i < kingdomCount; i++) {
        if (foreignKingdoms[i].name == kingdomName) {
            if (!foreignKingdoms[i].atWar && foreignKingdoms[i].relationLevel >= 2) { // Lowered from 3
                // Increase trade benefits
                market.getFood()->changeAmount(100 + (foreignKingdoms[i].relationLevel * 20));
                market.getWood()->changeAmount(50 + (foreignKingdoms[i].relationLevel * 10));
                market.getIron()->changeAmount(30 + (foreignKingdoms[i].relationLevel * 5));
                economy.setTreasuryGold(economy.getTreasuryGold() + 200 + (foreignKingdoms[i].relationLevel * 50));
                cout << "Trade deal boosts resources and treasury!" << endl;
                return true;
            }
            cout << "Cannot trade! Relations too low (need 2+) or at war." << endl;
            return false;
        }
    }
    cout << "Kingdom '" << kingdomName << "' not found!" << endl;
    return false;
}

void Diplomacy::updateDiplomacy(const Army& army, const Economy& economy) {
    // Update relations with foreign kingdoms based on various factors
    for (int i = 0; i < kingdomCount; i++) {
        if (foreignKingdoms[i].atWar) {
            // War affects relations
            foreignKingdoms[i].relationLevel = max(-10, foreignKingdoms[i].relationLevel - 1);

            // Simulate war outcomes
            int playerStrength = army.calculateStrength();
            int enemyStrength = foreignKingdoms[i].strength;

            // Random battle outcomes
            if (rand() % 100 < 20) { // 20% chance of a significant battle
                if (playerStrength > enemyStrength) {
                    // Player victory
                    cout << "Your forces defeat " << foreignKingdoms[i].name << " in battle!" << endl;
                    foreignKingdoms[i].strength = max(100, foreignKingdoms[i].strength - (playerStrength / 10));
                }
                else {
                    // Enemy victory
                    cout << "Your forces suffer defeat against " << foreignKingdoms[i].name << "!" << endl;
                    // Some army losses would be calculated here
                }
            }
        }
        else {
            // Natural relation drift
            int drift = (rand() % 3) - 1; // -1, 0, or 1
            foreignKingdoms[i].relationLevel = max(-10, min(10, foreignKingdoms[i].relationLevel + drift));
        }
    }
}

void Diplomacy::listKingdoms() const {
    // Display information about all foreign kingdoms
    cout << "\n===== Foreign Kingdoms =====" << endl;
    for (int i = 0; i < kingdomCount; i++) {
        cout << i + 1 << ". " << foreignKingdoms[i].name << ":" << endl;
        cout << "   Relation: ";

        if (foreignKingdoms[i].relationLevel >= 7) {
            cout << "Friendly";
        }
        else if (foreignKingdoms[i].relationLevel >= 3) {
            cout << "Cordial";
        }
        else if (foreignKingdoms[i].relationLevel >= 0) {
            cout << "Neutral";
        }
        else if (foreignKingdoms[i].relationLevel >= -3) {
            cout << "Suspicious";
        }
        else {
            cout << "Hostile";
        }

        cout << " (" << foreignKingdoms[i].relationLevel << ")" << endl;

        cout << "   Status: ";
        if (foreignKingdoms[i].atWar) {
            cout << "At War";
        }
        else if (foreignKingdoms[i].isAlly) {
            cout << "Allied";
        }
        else {
            cout << "Peaceful";
        }

        cout << endl;
        cout << "   Military Strength: " << foreignKingdoms[i].strength << endl;
    }
}

int Diplomacy::getRelationLevel(const string& kingdomName) const {
    // Get relation level with a specific kingdom
    for (int i = 0; i < kingdomCount; i++) {
        if (foreignKingdoms[i].name == kingdomName) {
            return foreignKingdoms[i].relationLevel;
        }
    }
    return 0; // Kingdom not found, return neutral
}

// ------------------
// Bank implementation
// ------------------

Bank::Bank(double initialInterestRate, int initialMaxLoan)
    : interestRate(initialInterestRate), maxLoanAmount(initialMaxLoan),
    currentLoans(0), corruptionLevel(0) {
}

Bank::~Bank() {}

double Bank::getInterestRate() const {
    return interestRate;
}

int Bank::getMaxLoanAmount() const {
    return maxLoanAmount;
}

int Bank::getCurrentLoans() const {
    return currentLoans;
}

int Bank::getCorruptionLevel() const {
    return corruptionLevel;
}

void Bank::setInterestRate(double rate) {
    interestRate = max(0.01, min(0.2, rate));
}

void Bank::setMaxLoanAmount(int amount) {
    maxLoanAmount = max(100, amount);
}

void Bank::setCorruptionLevel(int level) {
    corruptionLevel = max(0, min(100, level));
}

bool Bank::takeLoan(int amount, Economy& economy) {
    // Take a loan from the bank
    if (amount <= 0 || amount > maxLoanAmount) {
        return false;
    }

    // Add loan to debt
    economy.setDebt(economy.getDebt() + amount);

    // Add money to treasury
    economy.setTreasuryGold(economy.getTreasuryGold() + amount);

    // Track loan amount
    currentLoans += amount;

    return true;
}

bool Bank::repayLoan(int amount, Economy& economy) {
    // Repay a loan to the bank
    if (amount <= 0 || amount > economy.getTreasuryGold() || amount > economy.getDebt()) {
        return false;
    }

    // Deduct payment from treasury
    economy.setTreasuryGold(economy.getTreasuryGold() - amount);

    // Reduce debt
    economy.setDebt(economy.getDebt() - amount);

    // Track loan repayment
    currentLoans -= amount;
    if (currentLoans < 0) currentLoans = 0;

    return true;
}

void Bank::updateInterest(Economy& economy) {
    // Apply interest to outstanding debt
    int interest = static_cast<int>(economy.getDebt() * interestRate);
    economy.setDebt(economy.getDebt() + interest);
}

void Bank::attemptCorruption(Economy& economy, Population& population) {
    // Simulate corruption in the banking system
    if (corruptionLevel > 0) {
        // Chance of corruption scandal
        if (rand() % 100 < corruptionLevel) {
            int corruptionAmount = (economy.getTreasuryGold() * corruptionLevel) / 1000;
            economy.setTreasuryGold(economy.getTreasuryGold() - corruptionAmount);

            cout << "A corruption scandal has cost the treasury " << corruptionAmount << " gold!" << endl;

            // Corruption affects population happiness
            double happinessImpact = -0.05 - (static_cast<double>(corruptionLevel) / 1000.0);
            population.setHappiness(population.getHappiness() + happinessImpact);
        }
    }
}

// ---------------------------
// RandomEvents implementation
// ---------------------------

RandomEvents::RandomEvents(int chance)
    : eventChance(chance), lastEventTime(time(0)) {
}

RandomEvents::~RandomEvents() {}

bool RandomEvents::checkForEvent() {
    // Check if a random event should occur
    time_t currentTime = time(0);
    if (difftime(currentTime, lastEventTime) > 5) { // At least 5 seconds since last event
        if (rand() % 100 < eventChance) {
            lastEventTime = currentTime;
            return true;
        }
    }
    return false;
}

RandomEvents::EventType RandomEvents::generateEvent() const {
    // Generate a random event
    return static_cast<EventType>(rand() % 10); // 10 event types
}

void RandomEvents::applyEvent(EventType event, Kingdom& kingdom) {
    // Apply the effects of a random event
    switch (event) {
    case PLAGUE:
        describePlagueEvent(kingdom);
        break;
    case GOOD_HARVEST:
        describeGoodHarvestEvent(kingdom);
        break;
    case DROUGHT:
        describeDroughtEvent(kingdom);
        break;
    case FOREIGN_INVASION:
        describeForeignInvasionEvent(kingdom);
        break;
    case REBELLION:
        describeRebellionEvent(kingdom);
        break;
    case ASSASSINATION:
        describeAssassinationEvent(kingdom);
        break;
    case DISCOVERY:
        describeDiscoveryEvent(kingdom);
        break;
    case FESTIVAL:
        describeFestivalEvent(kingdom);
        break;
    case FIRE:
        describeFireEvent(kingdom);
        break;
    case EARTHQUAKE:
        describeEarthquakeEvent(kingdom);
        break;
    }
}

void RandomEvents::describePlagueEvent(Kingdom& kingdom) {
    cout << "\n===== EVENT: PLAGUE =====" << endl;
    cout << "A terrible plague sweeps through your kingdom!" << endl;

    // Reduce population
    int populationLoss = kingdom.getPopulation()->getTotal() / 10; // 10% loss
    kingdom.getPopulation()->setPeasants(kingdom.getPopulation()->getPeasants() -
        (populationLoss * 8 / 10));
    kingdom.getPopulation()->setMerchants(static_cast<int>(kingdom.getPopulation()->getMerchants() -
        (populationLoss * 1.5 / 10)));
    kingdom.getPopulation()->setNobles(static_cast<int>(kingdom.getPopulation()->getNobles() -
        (populationLoss * 0.5 / 10)));

    // Affect happiness
    kingdom.getPopulation()->setHappiness(kingdom.getPopulation()->getHappiness() - 0.2);

    cout << "The plague claims " << populationLoss << " lives." << endl;
    cout << "Population morale has decreased significantly." << endl;
}

void RandomEvents::describeGoodHarvestEvent(Kingdom& kingdom) {
    cout << "\n===== EVENT: GOOD HARVEST =====" << endl;
    cout << "A bountiful harvest blesses your kingdom!" << endl;

    // Increase food stocks
    int foodGain = kingdom.getPopulation()->getPeasants() * 2;
    kingdom.getMarket()->getFood()->changeAmount(foodGain);

    // Boost happiness
    kingdom.getPopulation()->setHappiness(kingdom.getPopulation()->getHappiness() + 0.15);

    cout << "Food stocks increase by " << foodGain << " units." << endl;
    cout << "The people rejoice at the abundance!" << endl;
}

void RandomEvents::describeDroughtEvent(Kingdom& kingdom) {
    cout << "\n===== EVENT: DROUGHT =====" << endl;
    cout << "A severe drought strikes your kingdom!" << endl;

    // Reduce food production
    int foodLoss = kingdom.getMarket()->getFood()->getAmount() / 3;
    kingdom.getMarket()->getFood()->changeAmount(-foodLoss);

    // Lower happiness
    kingdom.getPopulation()->setHappiness(kingdom.getPopulation()->getHappiness() - 0.1);

    cout << "Food stocks decrease by " << foodLoss << " units." << endl;
    cout << "The people grow anxious about the future." << endl;
}

void RandomEvents::describeForeignInvasionEvent(Kingdom& kingdom) {
    cout << "\n===== EVENT: FOREIGN INVASION =====" << endl;
    cout << "A neighboring kingdom invades your lands!" << endl;

    // Reduce army strength
    int armyLoss = kingdom.getArmy()->getTotal() / 10;
    kingdom.getArmy()->setInfantry(kingdom.getArmy()->getInfantry() - (armyLoss * 6 / 10));
    kingdom.getArmy()->setCavalry(kingdom.getArmy()->getCavalry() - (armyLoss * 2 / 10));
    kingdom.getArmy()->setArchers(kingdom.getArmy()->getArchers() - (armyLoss * 2 / 10));

    // Set war status
    kingdom.getArmy()->setWarStatus(true);

    // Lower morale
    kingdom.getArmy()->setMorale(kingdom.getArmy()->getMorale() - 0.15);

    cout << "Your army loses " << armyLoss << " troops in the conflict." << endl;
    cout << "The kingdom is now at war!" << endl;
}

void RandomEvents::describeRebellionEvent(Kingdom& kingdom) {
    cout << "\n===== EVENT: REBELLION =====" << endl;
    cout << "The people rise up against your rule!" << endl;

    // Reduce population and army
    int populationLoss = kingdom.getPopulation()->getTotal() / 10;
    kingdom.getPopulation()->setPeasants(kingdom.getPopulation()->getPeasants() - populationLoss);

    int armyLoss = kingdom.getArmy()->getTotal() / 10;
    kingdom.getArmy()->setInfantry(kingdom.getArmy()->getInfantry() - armyLoss);

    // Lower happiness and morale
    kingdom.getPopulation()->setHappiness(kingdom.getPopulation()->getHappiness() - 0.2);
    kingdom.getArmy()->setMorale(kingdom.getArmy()->getMorale() - 0.2);

    cout << "The rebellion claims " << populationLoss << " citizens and " << armyLoss << " soldiers." << endl;
    cout << "Your rule is questioned by many." << endl;
}

void RandomEvents::describeAssassinationEvent(Kingdom& kingdom) {
    cout << "\n===== EVENT: ASSASSINATION ATTEMPT =====" << endl;
    cout << "An assassin attempts to kill your ruler!" << endl;

    // 50% chance of success
    if (rand() % 2 == 0) {
        cout << "The attempt fails, but the kingdom is shaken!" << endl;
        kingdom.getPopulation()->setHappiness(kingdom.getPopulation()->getHappiness() - 0.1);
    }
    else {
        cout << "The ruler is gravely wounded and must be replaced!" << endl;
        // Replace ruler with a new one
        kingdom.setRuler(make_unique<King>("New King", 50, 50, 50, 50));
        kingdom.getPopulation()->setHappiness(kingdom.getPopulation()->getHappiness() - 0.3);
    }
}

void RandomEvents::describeDiscoveryEvent(Kingdom& kingdom) {
    cout << "\n===== EVENT: DISCOVERY =====" << endl;
    cout << "Your scholars uncover a valuable resource deposit!" << endl;

    // Randomly increase one resource
    int resourceType = rand() % 3;
    if (resourceType == 0) {
        int ironGain = 100 + (rand() % 100);
        kingdom.getMarket()->getIron()->changeAmount(ironGain);
        cout << "A new iron mine yields " << ironGain << " units!" << endl;
    }
    else if (resourceType == 1) {
        int woodGain = 200 + (rand() % 200);
        kingdom.getMarket()->getWood()->changeAmount(woodGain);
        cout << "A lush forest provides " << woodGain << " units of wood!" << endl;
    }
    else {
        int stoneGain = 150 + (rand() % 150);
        kingdom.getMarket()->getStone()->changeAmount(stoneGain);
        cout << "A quarry yields " << stoneGain << " units of stone!" << endl;
    }

    // Boost happiness
    kingdom.getPopulation()->setHappiness(kingdom.getPopulation()->getHappiness() + 0.1);
}

void RandomEvents::describeFestivalEvent(Kingdom& kingdom) {
    cout << "\n===== EVENT: FESTIVAL =====" << endl;
    cout << "A grand festival is held in the kingdom!" << endl;

    // Boost happiness
    kingdom.getPopulation()->setHappiness(kingdom.getPopulation()->getHappiness() + 0.2);

    // Small economic cost
    kingdom.getEconomy()->setTreasuryGold(kingdom.getEconomy()->getTreasuryGold() - 100);

    cout << "The festival costs 100 gold but greatly improves morale!" << endl;
}

void RandomEvents::describeFireEvent(Kingdom& kingdom) {
    cout << "\n===== EVENT: FIRE =====" << endl;
    cout << "A massive fire ravages part of the kingdom!" << endl;

    // Reduce resources
    int woodLoss = kingdom.getMarket()->getWood()->getAmount() / 4;
    int foodLoss = kingdom.getMarket()->getFood()->getAmount() / 5;
    kingdom.getMarket()->getWood()->changeAmount(-woodLoss);
    kingdom.getMarket()->getFood()->changeAmount(-foodLoss);

    // Lower happiness
    kingdom.getPopulation()->setHappiness(kingdom.getPopulation()->getHappiness() - 0.15);

    cout << "The fire destroys " << woodLoss << " wood and " << foodLoss << " food." << endl;
    cout << "The people mourn their losses." << endl;
}

void RandomEvents::describeEarthquakeEvent(Kingdom& kingdom) {
    cout << "\n===== EVENT: EARTHQUAKE =====" << endl;
    cout << "An earthquake shakes the kingdom to its core!" << endl;

    // Reduce stone and population
    int stoneLoss = kingdom.getMarket()->getStone()->getAmount() / 3;
    kingdom.getMarket()->getStone()->changeAmount(-stoneLoss);

    int populationLoss = kingdom.getPopulation()->getTotal() / 20;
    kingdom.getPopulation()->setPeasants(kingdom.getPopulation()->getPeasants() - populationLoss);

    // Lower happiness
    kingdom.getPopulation()->setHappiness(kingdom.getPopulation()->getHappiness() - 0.2);

    cout << "The earthquake destroys " << stoneLoss << " stone and claims " << populationLoss << " lives." << endl;
    cout << "The kingdom struggles to recover." << endl;
}

// ---------------------
// Kingdom implementation
// ---------------------

Kingdom::Kingdom(const string& kingdomName)
    : name(kingdomName), gameYear(1), score(0) {
    population = make_unique<Population>();
    army = make_unique<Army>();
    economy = make_unique<Economy>();
    market = make_unique<Market>();
    diplomacy = make_unique<Diplomacy>();
    bank = make_unique<Bank>();
    events = make_unique<RandomEvents>();
    ruler = make_unique<King>("Default King", 50, 50, 50, 50);
}

Kingdom::~Kingdom() {}

string Kingdom::getName() const {
    return name;
}

Population* Kingdom::getPopulation() const {
    return population.get();
}

Army* Kingdom::getArmy() const {
    return army.get();
}

Economy* Kingdom::getEconomy() const {
    return economy.get();
}

Market* Kingdom::getMarket() const {
    return market.get();
}

Diplomacy* Kingdom::getDiplomacy() const {
    return diplomacy.get();
}

Bank* Kingdom::getBank() const {
    return bank.get();
}

RandomEvents* Kingdom::getEvents() const {
    return events.get();
}

Leader* Kingdom::getRuler() const {
    return ruler.get();
}

int Kingdom::getGameYear() const {
    return gameYear;
}

int Kingdom::getScore() const {
    return score;
}

void Kingdom::setName(const string& newName) {
    name = newName;
}

void Kingdom::setRuler(unique_ptr<Leader> newRuler) {
    ruler = move(newRuler);
}

void Kingdom::setGameYear(int year) {
    gameYear = max(1, year);
}

void Kingdom::setScore(int newScore) {
    score = max(0, newScore);
}

void Kingdom::advanceYear() {
    cout << "\nAdvancing to year " << gameYear + 1 << "..." << endl;

    // Update all systems
    population->updatePopulation(*economy, *army);
    population->calculateHappiness(*economy, *army);
    army->updateMorale(*economy, *population);
    economy->updateEconomy(*population, *army);
    market->updatePrices(*economy);
    market->produceResources(*population);
    market->consumeResources(*population, *army);
    diplomacy->updateDiplomacy(*army, *economy);
    bank->updateInterest(*economy);
    bank->attemptCorruption(*economy, *population);

    // Apply leader effects
    ruler->applyEffects(*this);

    // Apply resource effects
    market->getFood()->applyEffects(*this);
    market->getIron()->applyEffects(*this);

    // Check for random events
    if (events->checkForEvent()) {
        RandomEvents::EventType event = events->generateEvent();
        events->applyEvent(event, *this);
    }

    // Check for rebellions or riots
    if (population->checkRebellion() || army->checkRebellion(*population) || economy->checkRiots(*population)) {
        cout << "\nWARNING: Unrest threatens the stability of your kingdom!" << endl;
        events->applyEvent(RandomEvents::REBELLION, *this);
    }

    // Collect taxes
    int taxes = economy->collectTaxes(*population);
    cout << "Collected " << taxes << " gold in taxes." << endl;

    // Increment year and calculate score
    gameYear++;
    calculateScore();
}

void Kingdom::calculateScore() {
    // Calculate score based on various factors
    score = (population->getTotal() * 10) +
        (army->getTotal() * 20) +
        (economy->getTreasuryGold() / 10) +
        (static_cast<int>(population->getHappiness() * 1000)) +
        (gameYear * 100);

    // Deduct points for debt and inflation
    score -= (economy->getDebt() / 5);
    score -= static_cast<int>(economy->getInflation() * 2000);
}

bool Kingdom::isGameOver() const {
    // Game over if population is too low, treasury is bankrupt, or massive debt
    return (population->getTotal() < 10) ||
        (economy->getTreasuryGold() <= 0 && economy->getDebt() > 5000) ||
        (population->getHappiness() < 0.1 && army->getMorale() < 0.1);
}

void Kingdom::displayStatus() const {
    cout << "\n===== Kingdom Status: " << name << " (Year " << gameYear << ") =====" << endl;
    cout << "Ruler: " << ruler->getName() << endl;
    cout << "Score: " << score << endl;

    cout << "\nPopulation:" << endl;
    cout << "  Peasants: " << population->getPeasants() << endl;
    cout << "  Merchants: " << population->getMerchants() << endl;
    cout << "  Nobles: " << population->getNobles() << endl;
    cout << "  Happiness: " << static_cast<int>(population->getHappiness() * 100) << "%" << endl;

    cout << "\nArmy:" << endl;
    cout << "  Infantry: " << army->getInfantry() << endl;
    cout << "  Cavalry: " << army->getCavalry() << endl;
    cout << "  Archers: " << army->getArchers() << endl;
    cout << "  Morale: " << static_cast<int>(army->getMorale() * 100) << "%" << endl;
    cout << "  Training Level: " << army->getTrainingLevel() << endl;
    cout << "  Status: " << (army->getWarStatus() ? "At War" : "At Peace") << endl;

    cout << "\nEconomy:" << endl;
    cout << "  Treasury: " << economy->getTreasuryGold() << " gold" << endl;
    cout << "  Debt: " << economy->getDebt() << " gold" << endl;
    cout << "  Inflation: " << static_cast<int>(economy->getInflation() * 100) << "%" << endl;

    cout << "\nMarket:" << endl;
    cout << "  Food: " << market->getFood()->getAmount() << " (Value: " << market->getFood()->getValue() << ")" << endl;
    cout << "  Wood: " << market->getWood()->getAmount() << " (Value: " << market->getWood()->getValue() << ")" << endl;
    cout << "  Stone: " << market->getStone()->getAmount() << " (Value: " << market->getStone()->getValue() << ")" << endl;
    cout << "  Iron: " << market->getIron()->getAmount() << " (Value: " << market->getIron()->getValue() << ")" << endl;
}

bool Kingdom::saveGame(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file to save game!" << endl;
        return false;
    }

    // Save basic kingdom data
    file << name << endl;
    file << gameYear << endl;
    file << score << endl;

    // Save population data
    file << population->getPeasants() << endl;
    file << population->getMerchants() << endl;
    file << population->getNobles() << endl;
    file << population->getHappiness() << endl;
    file << population->getGrowthRate() << endl;

    // Save army data
    file << army->getInfantry() << endl;
    file << army->getCavalry() << endl;
    file << army->getArchers() << endl;
    file << army->getMorale() << endl;
    file << army->getTrainingLevel() << endl;
    file << army->getWarStatus() << endl;

    // Save economy data
    file << economy->getTreasuryGold() << endl;
    file << economy->getDebt() << endl;
    file << economy->getPeasantTaxRate() << endl;
    file << economy->getMerchantTaxRate() << endl;
    file << economy->getNobleTaxRate() << endl;
    file << economy->getInflation() << endl;

    // Save market data
    file << market->getFood()->getAmount() << endl;
    file << market->getWood()->getAmount() << endl;
    file << market->getStone()->getAmount() << endl;
    file << market->getIron()->getAmount() << endl;

    // Save ruler data (simplified, assuming King)
    file << ruler->getName() << endl;
    file << dynamic_cast<King*>(ruler.get())->getRoyalBloodline() << endl;
    file << dynamic_cast<King*>(ruler.get())->getYearsInPower() << endl;

    file.close();
    cout << "Game saved successfully!" << endl;
    return true;
}

bool Kingdom::loadGame(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file to load game!" << endl;
        return false;
    }

    string rulerName;
    int royalBloodline, yearsInPower;
    string tempName;
    int tempInt;
    double tempDouble;

    // Load basic kingdom data
    getline(file, tempName);
    setName(tempName);
    file >> tempInt; setGameYear(tempInt);
    file >> tempInt; setScore(tempInt);
    file.ignore(); // Clear newline

    // Load population data
    file >> tempInt; population->setPeasants(tempInt);
    file >> tempInt; population->setMerchants(tempInt);
    file >> tempInt; population->setNobles(tempInt);
    file >> tempDouble; population->setHappiness(tempDouble);
    file >> tempDouble; population->setGrowthRate(tempDouble);

    // Load army data
    file >> tempInt; army->setInfantry(tempInt);
    file >> tempInt; army->setCavalry(tempInt);
    file >> tempInt; army->setArchers(tempInt);
    file >> tempDouble; army->setMorale(tempDouble);
    file >> tempInt; army->setTrainingLevel(tempInt);
    file >> tempInt; army->setWarStatus(tempInt != 0);

    // Load economy data
    file >> tempInt; economy->setTreasuryGold(tempInt);
    file >> tempInt; economy->setDebt(tempInt);
    file >> tempDouble; economy->setPeasantTaxRate(tempDouble);
    file >> tempDouble; economy->setMerchantTaxRate(tempDouble);
    file >> tempDouble; economy->setNobleTaxRate(tempDouble);
    file >> tempDouble; economy->setInflation(tempDouble);

    // Load market data
    file >> tempInt; market->getFood()->setAmount(tempInt);
    file >> tempInt; market->getWood()->setAmount(tempInt);
    file >> tempInt; market->getStone()->setAmount(tempInt);
    file >> tempInt; market->getIron()->setAmount(tempInt);

    // Load ruler data
    file.ignore();
    getline(file, rulerName);
    file >> royalBloodline >> yearsInPower;
    setRuler(make_unique<King>(rulerName, 50, 50, 50, royalBloodline));
    dynamic_cast<King*>(ruler.get())->incrementYearsInPower(); // Simplified restoration

    file.close();
    cout << "Game loaded successfully!" << endl;
    cout << "Kingdom: " << name << ", Year: " << gameYear << ", Score: " << score << endl;
    return true;
}

void Kingdom::handleEvent(RandomEvents::EventType event) {
    events->applyEvent(event, *this);
}

void Kingdom::holdElections() {
    cout << "\n===== ELECTIONS =====" << endl;
    cout << "The people demand a new ruler!" << endl;

    // Randomly select a new ruler type
    int leaderType = rand() % 3;
    if (leaderType == 0) {
        setRuler(make_unique<King>("Elected King", 60, 50, 50, 60));
        cout << "A new King is crowned!" << endl;
    }
    else if (leaderType == 1) {
        setRuler(make_unique<Commander>("Elected Commander", 50, 50, 70, 60));
        cout << "A military Commander takes charge!" << endl;
    }
    else {
        setRuler(make_unique<GuildLeader>("Elected Guild Leader", 50, 60, 50, "Merchants", 60));
        cout << "A Guild Leader rises to power!" << endl;
    }

    // Boost happiness due to change
    population->setHappiness(population->getHappiness() + 0.1);
}

// -------------------------
// Utility functions
// -------------------------

void displayMainMenu() {
    cout << "\n===== Stronghold: Kingdom Management =====" << endl;
    cout << "1. Advance Year" << endl;
    cout << "2. Display Status" << endl;
    cout << "3. Manage Resources" << endl;
    cout << "4. Manage Army" << endl;
    cout << "5. Manage Economy" << endl;
    cout << "6. Manage Diplomacy" << endl;
    cout << "7. Manage Bank" << endl;
    cout << "8. Hold Elections" << endl;
    cout << "9. Perform Ruler Action" << endl;
    cout << "10. Trigger Random Event" << endl;
    cout << "11. Save Game" << endl;
    cout << "12. Load Game" << endl;
    cout << "13. Exit" << endl;
    cout << "Enter choice: ";
}

bool processMenuChoice(int choice, Kingdom& kingdom) {
    string filename;
    switch (choice) {
    case 1:
        kingdom.advanceYear();
        pauseScreen();
        break;
    case 2:
        kingdom.displayStatus();
        pauseScreen();
        break;
    case 3:
        displayResourceManagementMenu(kingdom);
        break;
    case 4:
        displayArmyMenu(kingdom);
        break;
    case 5:
        displayEconomyMenu(kingdom);
        break;
    case 6:
        displayDiplomacyMenu(kingdom);
        break;
    case 7:
        displayBankMenu(kingdom);
        break;
    case 8:
        kingdom.holdElections();
        pauseScreen();
        break;
    case 9:
        kingdom.getRuler()->specialAction(kingdom);
        pauseScreen();
        break;
    case 10:
        kingdom.handleEvent(kingdom.getEvents()->generateEvent());
        pauseScreen();
        break;
    case 11:
        cout << "Enter save file name (e.g., savegame.txt): ";
        getline(cin, filename);
        if (filename.empty()) {
            cout << "Error: Filename cannot be empty!" << endl;
        }
        else {
            kingdom.saveGame(filename);
        }
        pauseScreen();
        break;
    case 12:
        cout << "Enter load file name (e.g., savegame.txt): ";
        getline(cin, filename);
        if (filename.empty()) {
            cout << "Error: Filename cannot be empty!" << endl;
        }
        else {
            kingdom.loadGame(filename);
        }
        pauseScreen();
        break;
    case 13:
        cout << "Exiting game..." << endl;
        return false;
    default:
        cout << "Invalid choice!" << endl;
        pauseScreen();
    }
    return true;
}

bool validateIntInput(int& value, const string& prompt, int min, int max) {
    cout << prompt;
    cin >> value;
    if (cin.fail() || value < min || value > max) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input! Must be between " << min << " and " << max << "." << endl;
        return false;
    }
    cin.ignore();
    return true;
}

bool validateDoubleInput(double& value, const string& prompt, double min, double max) {
    cout << prompt;
    cin >> value;
    if (cin.fail() || value < min || value > max) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input! Must be between " << min << " and " << max << "." << endl;
        return false;
    }
    cin.ignore();
    return true;
}

bool validateStringInput(string& value, const string& prompt, int minLength, int maxLength) {
    cout << prompt;
    getline(cin, value);
    if (value.length() < minLength || value.length() > maxLength) {
        cout << "Invalid input! Length must be between " << minLength << " and " << maxLength << "." << endl;
        return false;
    }
    return true;
}

void displayResourceManagementMenu(Kingdom& kingdom) {
    int choice;
    int amount;
    string resourceType;

    do {
        cout << "\n===== Resource Management =====" << endl;
        cout << "1. Buy Resources" << endl;
        cout << "2. Sell Resources" << endl;
        cout << "3. View Market" << endl;
        cout << "4. Back" << endl;
        cout << "Enter choice: ";

        if (validateIntInput(choice, "", 1, 4)) {
            switch (choice) {
            case 1:
                cout << "Enter resource type (Food/Wood/Stone/Iron): ";
                getline(cin, resourceType);
                if (validateIntInput(amount, "Enter amount to buy: ", 1, 1000)) {
                    if (kingdom.getMarket()->buyResource(resourceType, amount, *kingdom.getEconomy())) {
                        cout << "Purchased " << amount << " " << resourceType << "!" << endl;
                    }
                    else {
                        cout << "Failed to buy! Check funds or resource type." << endl;
                    }
                }
                break;
            case 2:
                cout << "Enter resource type (Food/Wood/Stone/Iron): ";
                getline(cin, resourceType);
                if (validateIntInput(amount, "Enter amount to sell: ", 1, 1000)) {
                    if (kingdom.getMarket()->sellResource(resourceType, amount, *kingdom.getEconomy())) {
                        cout << "Sold " << amount << " " << resourceType << "!" << endl;
                    }
                    else {
                        cout << "Failed to sell! Check stock or resource type." << endl;
                    }
                }
                break;
            case 3:
                kingdom.displayStatus();
                break;
            case 4:
                return;
            }
        }
    } while (true);
}

void displayArmyMenu(Kingdom& kingdom) {
    int choice;

    do {
        cout << "\n===== Army Management =====" << endl;
        cout << "1. Train Army" << endl;
        cout << "2. Recruit Infantry" << endl;
        cout << "3. Recruit Cavalry" << endl;
        cout << "4. Recruit Archers" << endl;
        cout << "5. Back" << endl;
        cout << "Enter choice: ";

        if (validateIntInput(choice, "", 1, 5)) {
            switch (choice) {
            case 1:
                kingdom.getArmy()->trainArmy();
                break;
            case 2:
                int infantry;
                if (validateIntInput(infantry, "Enter number of infantry to recruit: ", 1, 100)) {
                    kingdom.getArmy()->setInfantry(kingdom.getArmy()->getInfantry() + infantry);
                    kingdom.getEconomy()->setTreasuryGold(kingdom.getEconomy()->getTreasuryGold() - infantry * 10);
                    cout << "Recruited " << infantry << " infantry!" << endl;
                }
                break;
            case 3:
                int cavalry;
                if (validateIntInput(cavalry, "Enter number of cavalry to recruit: ", 1, 50)) {
                    kingdom.getArmy()->setCavalry(kingdom.getArmy()->getCavalry() + cavalry);
                    kingdom.getEconomy()->setTreasuryGold(kingdom.getEconomy()->getTreasuryGold() - cavalry * 20);
                    cout << "Recruited " << cavalry << " cavalry!" << endl;
                }
                break;
            case 4:
                int archers;
                if (validateIntInput(archers, "Enter number of archers to recruit: ", 1, 50)) {
                    kingdom.getArmy()->setArchers(kingdom.getArmy()->getArchers() + archers);
                    kingdom.getEconomy()->setTreasuryGold(kingdom.getEconomy()->getTreasuryGold() - archers * 15);
                    cout << "Recruited " << archers << " archers!" << endl;
                }
                break;
            case 5:
                return;
            }
        }
    } while (true);
}

void displayEconomyMenu(Kingdom& kingdom) {
    int choice;
    double rate;

    do {
        cout << "\n===== Economy Management =====" << endl;
        cout << "1. Adjust Peasant Tax Rate" << endl;
        cout << "2. Adjust Merchant Tax Rate" << endl;
        cout << "3. Adjust Noble Tax Rate" << endl;
        cout << "4. Back" << endl;
        cout << "Enter choice: ";

        if (validateIntInput(choice, "", 1, 4)) {
            switch (choice) {
            case 1:
                if (validateDoubleInput(rate, "Enter new peasant tax rate (0.0-0.5): ", 0.0, 0.5)) {
                    kingdom.getEconomy()->setPeasantTaxRate(rate);
                    cout << "Peasant tax rate set to " << rate << "!" << endl;
                }
                break;
            case 2:
                if (validateDoubleInput(rate, "Enter new merchant tax rate (0.0-0.5): ", 0.0, 0.5)) {
                    kingdom.getEconomy()->setMerchantTaxRate(rate);
                    cout << "Merchant tax rate set to " << rate << "!" << endl;
                }
                break;
            case 3:
                if (validateDoubleInput(rate, "Enter new noble tax rate (0.0-0.5): ", 0.0, 0.5)) {
                    kingdom.getEconomy()->setNobleTaxRate(rate);
                    cout << "Noble tax rate set to " << rate << "!" << endl;
                }
                break;
            case 4:
                return;
            }
        }
    } while (true);
}

void displayDiplomacyMenu(Kingdom& kingdom) {
    int choice;
    string kingdomName;

    do {
        cout << "\n===== Diplomacy Management =====" << endl;
        cout << "1. List Foreign Kingdoms" << endl;
        cout << "2. Improve Relations" << endl;
        cout << "3. Declare War" << endl;
        cout << "4. Sign Peace Treaty" << endl;
        cout << "5. Form Alliance" << endl;
        cout << "6. Establish Trade" << endl;
        cout << "7. Engage in Battle" << endl;
        cout << "8. Back" << endl;
        cout << "Enter choice: ";

        if (validateIntInput(choice, "", 1, 8)) {
            switch (choice) {
            case 1:
                kingdom.getDiplomacy()->listKingdoms();
                pauseScreen();
                break;
            case 2:
                kingdom.getDiplomacy()->listKingdoms();
                cout << "Enter kingdom name: ";
                getline(cin, kingdomName);
                if (kingdom.getDiplomacy()->improveRelations(kingdomName, *kingdom.getEconomy())) {
                    cout << "Relations with " << kingdomName << " improved!" << endl;
                }
                else {
                    cout << "Failed to improve relations! Check funds or kingdom name." << endl;
                }
                pauseScreen();
                break;
            case 3:
                kingdom.getDiplomacy()->listKingdoms();
                cout << "Enter kingdom name to declare war on: ";
                getline(cin, kingdomName);
                if (kingdom.getDiplomacy()->declareWar(kingdomName, *kingdom.getArmy())) {
                    cout << "War declared on " << kingdomName << "!" << endl;
                }
                else {
                    cout << "Failed to declare war! Already at war or invalid kingdom." << endl;
                }
                pauseScreen();
                break;
            case 4:
                kingdom.getDiplomacy()->listKingdoms();
                cout << "Enter kingdom name to sign peace with: ";
                getline(cin, kingdomName);
                if (kingdom.getDiplomacy()->signPeace(kingdomName, *kingdom.getEconomy())) {
                    cout << "Peace signed with " << kingdomName << "!" << endl;
                }
                else {
                    cout << "Failed to sign peace! Not at war or insufficient funds." << endl;
                }
                pauseScreen();
                break;
            case 5:
                kingdom.getDiplomacy()->listKingdoms();
                cout << "Enter kingdom name to form alliance with: ";
                getline(cin, kingdomName);
                if (kingdom.getDiplomacy()->formAlliance(kingdomName)) {
                    cout << "Alliance formed with " << kingdomName << "!" << endl;
                }
                else {
                    cout << "Failed to form alliance! Relations too low or at war." << endl;
                }
                pauseScreen();
                break;
            case 6:                   
                kingdom.getDiplomacy()->listKingdoms();
                cout << "Enter kingdom name to establish trade with: ";
                getline(cin, kingdomName);
                if (kingdom.getDiplomacy()->establishTrade(kingdomName, *kingdom.getMarket(), *kingdom.getEconomy())) {
                    cout << "Trade established with " << kingdomName << "!" << endl;
                }
                else {
                    cout << "Failed to establish trade! Relations too low or at war." << endl;
                }
                pauseScreen();
                break;
            case 7: {
                kingdom.getDiplomacy()->listKingdoms();
                cout << "Enter kingdom name to battle: ";
                getline(cin, kingdomName);
                bool atWar = false;
                for (int i = 0; i < kingdom.getDiplomacy()->getKingdomCount(); i++) {
                    if (kingdom.getDiplomacy()->getForeignKingdoms()[i].name == kingdomName &&
                        kingdom.getDiplomacy()->getForeignKingdoms()[i].atWar) {
                        atWar = true;
                        int playerStrength = kingdom.getArmy()->calculateStrength();
                        int enemyStrength = kingdom.getDiplomacy()->getForeignKingdoms()[i].strength;
                        cout << "\nBattle against " << kingdomName << " begins!" << endl;
                        if (playerStrength > enemyStrength) {
                            cout << "Victory! Your forces crush the enemy!" << endl;
                            kingdom.getDiplomacy()->getForeignKingdomsMutable()[i].strength = max(100, enemyStrength - (playerStrength / 10));
                            kingdom.getArmy()->setMorale(kingdom.getArmy()->getMorale() + 0.1);
                        }
                        else {
                            cout << "Defeat! Your army suffers heavy losses!" << endl;
                            int armyLoss = kingdom.getArmy()->getTotal() / 10;
                            kingdom.getArmy()->setInfantry(kingdom.getArmy()->getInfantry() - (armyLoss * 6 / 10));
                            kingdom.getArmy()->setCavalry(kingdom.getArmy()->getCavalry() - (armyLoss * 2 / 10));
                            kingdom.getArmy()->setArchers(kingdom.getArmy()->getArchers() - (armyLoss * 2 / 10));
                            kingdom.getArmy()->setMorale(kingdom.getArmy()->getMorale() - 0.1);
                        }
                        break;
                    }
                }
                if (!atWar) {
                    cout << "Cannot battle! Not at war with " << kingdomName << " or invalid kingdom." << endl;
                }
                pauseScreen();
                break;
            }
            case 8:
                return;
            }
        }
    } while (true);
}

void displayBankMenu(Kingdom& kingdom) {
    int choice;
    int amount;

    do {
        cout << "\n===== Bank Management =====" << endl;
        cout << "1. Take Loan" << endl;
        cout << "2. Repay Loan" << endl;
        cout << "3. View Bank Status" << endl;
        cout << "4. Back" << endl;
        cout << "Enter choice: ";

        if (validateIntInput(choice, "", 1, 4)) {
            switch (choice) {
            case 1:
                if (validateIntInput(amount, "Enter loan amount: ", 1, kingdom.getBank()->getMaxLoanAmount())) {
                    if (kingdom.getBank()->takeLoan(amount, *kingdom.getEconomy())) {
                        cout << "Loan of " << amount << " gold taken!" << endl;
                    }
                    else {
                        cout << "Failed to take loan! Amount too high." << endl;
                    }
                }
                break;
            case 2:
                if (validateIntInput(amount, "Enter amount to repay: ", 1, kingdom.getEconomy()->getDebt())) {
                    if (kingdom.getBank()->repayLoan(amount, *kingdom.getEconomy())) {
                        cout << "Repaid " << amount << " gold of loan!" << endl;
                    }
                    else {
                        cout << "Failed to repay loan! Check funds or debt." << endl;
                    }
                }
                break;
            case 3:
                cout << "\nBank Status:" << endl;
                cout << "  Interest Rate: " << kingdom.getBank()->getInterestRate() * 100 << "%" << endl;
                cout << "  Current Loans: " << kingdom.getBank()->getCurrentLoans() << " gold" << endl;
                cout << "  Corruption Level: " << kingdom.getBank()->getCorruptionLevel() << endl;
                break;
            case 4:
                return;
            }
        }
    } while (true);
}

void clearScreen() {
    system("cls");
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(10000, '\n');
}