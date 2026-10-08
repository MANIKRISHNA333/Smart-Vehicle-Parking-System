#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

// =====================================================
// INPUT VALIDATION FUNCTIONS
// =====================================================

// Get a valid integer
int getInteger() {
  int value;
  while (true) {
    cin >> value;
    if (!cin.fail()) {
      cin.ignore(1000, '\n');
      return value;
    }
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "Invalid input! Please enter a number: ";
  }
}

// Get a positive number
double getPositiveNumber() {
  double value;
  while (true) {
    cin >> value;
    if (!cin.fail() && value > 0) {
      cin.ignore(1000, '\n');
      return value;
    }
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "Invalid input! Enter a positive number: ";
  }
}

// =====================================================
// VEHICLE BASE CLASS
// =====================================================

class Vehicle {
 protected:
  string vehicleNumber;
  string ownerName;
  int slotNumber;

 public:
  Vehicle(string number, string owner) {
    vehicleNumber = number;
    ownerName = owner;
    slotNumber = 0;
  }

  // Pure virtual functions
  virtual string getVehicleType() = 0;
  virtual double calculateFee(double hours) = 0;

  string getVehicleNumber() {
    return vehicleNumber;
  }
  string getOwnerName() {
    return ownerName;
  }
  int getSlotNumber() {
    return slotNumber;
  }

  void setSlotNumber(int slot) {
    slotNumber = slot;
  }

  virtual ~Vehicle() {
  }
};

// =====================================================
// BIKE CLASS
// =====================================================

class Bike : public Vehicle {
 public:
  Bike(string number, string owner) : Vehicle(number, owner) {
  }

  string getVehicleType() {
    return "Bike";
  }
  double calculateFee(double hours) {
    return hours * 20;
  }
};

// =====================================================
// CAR CLASS
// =====================================================

class Car : public Vehicle {
 public:
  Car(string number, string owner) : Vehicle(number, owner) {
  }

  string getVehicleType() {
    return "Car";
  }
  double calculateFee(double hours) {
    return hours * 40;
  }
};

// =====================================================
// SUV CLASS
// =====================================================

class SUV : public Vehicle {
 public:
  SUV(string number, string owner) : Vehicle(number, owner) {
  }

  string getVehicleType() {
    return "SUV";
  }
  double calculateFee(double hours) {
    return hours * 60;
  }
};

// =====================================================
// PARKING SLOT CLASS
// =====================================================

class ParkingSlot {
 private:
  int slotNumber;
  bool occupied;

 public:
  ParkingSlot() {
    slotNumber = 0;
    occupied = false;
  }

  ParkingSlot(int number) {
    slotNumber = number;
    occupied = false;
  }

  int getSlotNumber() {
    return slotNumber;
  }
  bool isOccupied() {
    return occupied;
  }

  void occupy() {
    occupied = true;
  }
  void release() {
    occupied = false;
  }
};

// =====================================================
// PARKING SYSTEM CLASS
// =====================================================

class ParkingSystem {
 private:
  ParkingSlot slots[5];
  Vehicle* vehicles[5];
  int vehicleCount;

 public:
  // -------------------------------------------------
  // CONSTRUCTOR
  // -------------------------------------------------
  ParkingSystem() {
    vehicleCount = 0;
    for (int i = 0; i < 5; i++) {
      slots[i] = ParkingSlot(i + 1);
      vehicles[i] = NULL;
    }
  }

  // -------------------------------------------------
  // FIND AVAILABLE SLOT
  // -------------------------------------------------
  int findAvailableSlot() {
    for (int i = 0; i < 5; i++) {
      if (!slots[i].isOccupied()) {
        return i;
      }
    }
    return -1;
  }

  // -------------------------------------------------
  // VEHICLE ENTRY
  // -------------------------------------------------
  void vehicleEntry() {
    if (vehicleCount >= 5) {
      cout << "\nParking is full!\n";
      return;
    }

    string number;
    string owner;
    int type;

    cout << "\nEnter Vehicle Number: ";
    cin >> number;

    // --- Check duplicate vehicle ---
    for (int i = 0; i < vehicleCount; i++) {
      if (vehicles[i]->getVehicleNumber() == number) {
        cout << "\nVehicle already exists in parking!\n";
        return;
      }
    }

    // --- Owner name ---
    cout << "Enter Owner Name: ";

    // Clear the newline left by "cin >> number"
    // so getline does not read an empty line
    cin.ignore(1000, '\n');

    do {
      getline(cin, owner);

      if (owner.empty()) {
        cout << "Owner name cannot be empty. Enter again: ";
      }
    } while (owner.empty());

    // --- Vehicle type ---
    while (true) {
      cout << "\nSelect Vehicle Type:\n";
      cout << "1. Bike\n";
      cout << "2. Car\n";
      cout << "3. SUV\n";
      cout << "Enter choice: ";

      type = getInteger();

      if (type >= 1 && type <= 3) {
        break;
      }

      cout << "Invalid vehicle type! Please choose 1, 2, or 3.\n";
    }

    // --- Create vehicle object ---
    Vehicle* newVehicle = NULL;

    if (type == 1) {
      newVehicle = new Bike(number, owner);
    } else if (type == 2) {
      newVehicle = new Car(number, owner);
    } else if (type == 3) {
      newVehicle = new SUV(number, owner);
    }

    // --- Find available slot ---
    int slotIndex = findAvailableSlot();

    if (slotIndex == -1) {
      cout << "\nNo parking slot available!\n";
      delete newVehicle;
      return;
    }

    // --- Occupy slot ---
    slots[slotIndex].occupy();
    newVehicle->setSlotNumber(slots[slotIndex].getSlotNumber());

    // --- Store vehicle ---
    vehicles[vehicleCount] = newVehicle;
    vehicleCount++;

    // --- Display entry details ---
    cout << "\nVehicle Entry Successful!\n";
    cout << "Vehicle Number : " << newVehicle->getVehicleNumber() << endl;
    cout << "Owner Name     : " << newVehicle->getOwnerName() << endl;
    cout << "Vehicle Type   : " << newVehicle->getVehicleType() << endl;
    cout << "Assigned Slot  : " << newVehicle->getSlotNumber() << endl;
  }

  // -------------------------------------------------
  // PARKING STATUS
  // -------------------------------------------------
  void parkingStatus() {
    cout << "\n========== PARKING STATUS ==========\n";

    for (int i = 0; i < 5; i++) {
      cout << "\nSlot " << slots[i].getSlotNumber() << " : ";

      if (!slots[i].isOccupied()) {
        cout << "Available" << endl;
        continue;
      }

      cout << "Occupied" << endl;

      // Find vehicle in this slot
      for (int j = 0; j < vehicleCount; j++) {
        if (vehicles[j]->getSlotNumber() == slots[i].getSlotNumber()) {
          cout << "  Vehicle Number : " << vehicles[j]->getVehicleNumber()
               << endl;
          cout << "  Owner Name     : " << vehicles[j]->getOwnerName() << endl;
          cout << "  Vehicle Type   : " << vehicles[j]->getVehicleType()
               << endl;
          break;
        }
      }
    }

    cout << "\n=====================================\n";
  }

  // -------------------------------------------------
  // VEHICLE SEARCH
  // -------------------------------------------------
  void searchVehicle() {
    string number;

    cout << "\nEnter Vehicle Number to Search: ";
    cin >> number;

    for (int i = 0; i < vehicleCount; i++) {
      if (vehicles[i]->getVehicleNumber() == number) {
        cout << "\nVehicle Found!\n";
        cout << "Vehicle Number : " << vehicles[i]->getVehicleNumber() << endl;
        cout << "Owner Name     : " << vehicles[i]->getOwnerName() << endl;
        cout << "Vehicle Type   : " << vehicles[i]->getVehicleType() << endl;
        cout << "Parking Slot   : " << vehicles[i]->getSlotNumber() << endl;
        cout << "Status         : Parked\n";
        return;
      }
    }

    cout << "\nVehicle not found.\n";
  }

  // -------------------------------------------------
  // SAVE PARKING HISTORY
  // -------------------------------------------------
  void saveHistory(Vehicle* vehicle, double hours, double fee) {
    ofstream historyFile("parking_history.txt", ios::app);

    if (!historyFile) {
      cout << "\nError opening history file!\n";
      return;
    }

    historyFile << "========================================\n";
    historyFile << "Vehicle Number : " << vehicle->getVehicleNumber() << endl;
    historyFile << "Owner Name     : " << vehicle->getOwnerName() << endl;
    historyFile << "Vehicle Type   : " << vehicle->getVehicleType() << endl;
    historyFile << "Parking Slot   : " << vehicle->getSlotNumber() << endl;
    historyFile << "Duration       : " << hours << " hours" << endl;

    historyFile << fixed << setprecision(2);
    historyFile << "Total Fee      : Rs. " << fee << endl;
    historyFile << "========================================\n\n";

    historyFile.close();
  }

  // -------------------------------------------------
  // VEHICLE EXIT
  // -------------------------------------------------
  void vehicleExit() {
    string number;

    cout << "\nEnter Vehicle Number for Exit: ";
    cin >> number;

    // --- Search vehicle ---
    for (int i = 0; i < vehicleCount; i++) {
      if (vehicles[i]->getVehicleNumber() != number) {
        continue;
      }

      cout << "Enter Parking Duration (hours): ";
      double hours = getPositiveNumber();

      // --- Calculate fee ---
      double fee = vehicles[i]->calculateFee(hours);

      // --- Save history ---
      saveHistory(vehicles[i], hours, fee);

      // --- Print receipt ---
      cout << "\n========== PARKING RECEIPT ==========\n";
      cout << "Vehicle Number : " << vehicles[i]->getVehicleNumber() << endl;
      cout << "Owner Name     : " << vehicles[i]->getOwnerName() << endl;
      cout << "Vehicle Type   : " << vehicles[i]->getVehicleType() << endl;
      cout << "Parking Slot   : " << vehicles[i]->getSlotNumber() << endl;
      cout << "Duration       : " << hours << " hours" << endl;

      cout << fixed << setprecision(2);
      cout << "Total Fee      : Rs. " << fee << endl;
      cout << "=====================================\n";

      // --- Release parking slot ---
      int slot = vehicles[i]->getSlotNumber();

      for (int j = 0; j < 5; j++) {
        if (slots[j].getSlotNumber() == slot) {
          slots[j].release();
          break;
        }
      }

      // --- Delete vehicle object ---
      delete vehicles[i];

      // --- Shift vehicle array ---
      for (int j = i; j < vehicleCount - 1; j++) {
        vehicles[j] = vehicles[j + 1];
      }

      vehicles[vehicleCount - 1] = NULL;
      vehicleCount--;

      cout << "\nVehicle exited successfully.\n";
      return;
    }

    cout << "\nVehicle not found.\n";
  }

  // -------------------------------------------------
  // SHOW PARKING HISTORY
  // -------------------------------------------------
  void showHistory() {
    ifstream historyFile("parking_history.txt");

    if (!historyFile) {
      cout << "\nNo parking history available.\n";
      return;
    }

    string line;

    cout << "\n========== PARKING HISTORY ==========\n";

    while (getline(historyFile, line)) {
      cout << line << endl;
    }

    historyFile.close();
  }
};

// =====================================================
// MAIN FUNCTION
// =====================================================

int main() {
  ParkingSystem parking;
  int choice;

  do {
    cout << "\n\n=====================================\n";
    cout << "     SMART VEHICLE PARKING SYSTEM\n";
    cout << "=====================================\n";
    cout << "1. Vehicle Entry\n";
    cout << "2. Parking Status\n";
    cout << "3. Vehicle Search\n";
    cout << "4. Vehicle Exit\n";
    cout << "5. Parking History\n";
    cout << "6. Exit Program\n";
    cout << "Enter your choice: ";

    // Validated menu input
    choice = getInteger();

    switch (choice) {
      case 1:
        parking.vehicleEntry();
        break;

      case 2:
        parking.parkingStatus();
        break;

      case 3:
        parking.searchVehicle();
        break;

      case 4:
        parking.vehicleExit();
        break;

      case 5:
        parking.showHistory();
        break;

      case 6:
        cout << "\nThank you for using Smart Vehicle Parking System!\n";
        break;

      default:
        cout << "\nInvalid choice! Please select 1 to 6.\n";
    }

  } while (choice != 6);

  return 0;
}
