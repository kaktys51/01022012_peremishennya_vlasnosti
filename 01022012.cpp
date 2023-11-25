#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>


using namespace std;

class Property
{
protected:
	string name;
	int value;
	Property* address;
public:
	Property() : name("default"), value(500), address(this) {}
	Property(string _name, int _value) : name(_name), value(_value), address(this) {}
	Property(Property&& a) noexcept : name(move(a.name)), value(move(a.value)), address(a.address) { a.address = nullptr; }
	~Property() {}

	int getPrice() { return value; }
	void printIndex()
	{
		cout << address << endl;
	}
	void printPropInfo()
	{
		cout << "| Property name: \t" << name << "\t |" << endl;
		cout << "| Value: \t\t" << value << "\t |" << endl;
		cout << "| Address: \t" << address << " |" << endl;
	}
	Property* getAddress() { return address; }
	Property& operator=(Property&& a)noexcept
	{
		if (&a == this) return *this;

		name = move(a.name);
		value = move(a.value);
		address = exchange(a.address, nullptr);
		return *this;
	}
	bool operator==(const Property& a)const
	{
		return (name == a.name) && (value == a.value) && (address == a.address);
	}
};

class Owner
{
protected:
	string firstName;
	string lastName;
	int id;
	double wallet;
	vector<Property*> properties;

	int idGen()
	{
		int a = 1000000 + std::rand() % (9999999 - 1000000 + 1);
		return a;
	}
public:
	Owner(string last, string first, double _wallet) : lastName(last), firstName(first), wallet(_wallet) { id = idGen(); }
	void buyProp(Owner& seller) 
	{
		if (seller.getAmountProp() == 0)
		{
			cout << seller.lastName << " " << seller.firstName << " doesn't have any property to sell!" << endl;
			return;
		}

		int countProp = seller.getAmountProp();
		int choise = 0;
		bool confirm = false;

		cout << "Seller: " << seller.lastName << " " << seller.firstName << endl;
		cout << "Properties owned: " << countProp << endl;
		cout << endl;

		printOwnedProperties(seller);

		cout << "Type in the number of property you want to buy below:" << endl;
		do
		{
			cin >> choise;
			if (choise > countProp || choise < 1)
			{
				cout << "Invalid nuber of property!" << endl;
				cout << "Please try again:" << endl;
				confirm = false;
			}
			else if (wallet < seller.properties[choise - 1]->getPrice())
			{
				cout << "You do not have enough money to buy this property! " << endl;
				cout << "Please try to select somthing else! " << endl;
				confirm = false;
			}
			else confirm = true;
		} while (confirm == false);

		cout << "\nThis property cost is: " << seller.properties[choise - 1]->getPrice() << endl;
		cout << "Your balance: \t" << wallet << endl;
		cout << "\nConfirm transaction?" << endl;
		cout << "Type: 0 - to dicline or 1 - to proceed" << endl;

		int tranch = 0;
		do
		{
			cin >> tranch;
			if (tranch == 0)
			{
				cout << "\n Transaction aborted." << endl;
				return;
			}
			else if (tranch != 0 && tranch != 1) cout << "Invalid option! \n Please try again:" << endl;
		} while (tranch != 1);

		wallet -= seller.properties[choise - 1]->getPrice();         // транзацкции по счетам
		seller.wallet += seller.properties[choise - 1]->getPrice();  // транзацкции по счетам

		properties.push_back(move(seller.properties[choise - 1]));       //транзакции по иммуществу
		seller.properties.erase(seller.properties.begin() + choise - 1); //транзакции по иммуществу
		
		cout << "Congratulation! You successfully bought new property" << endl;
		cout << "Your balance after transaction: " << wallet << endl;
		cout << "Updated list yours properties: " << endl;

		printOwnedProperties();

	}
	int getAmountProp() { return properties.size(); }
	void printOwnedProperties()
	{
		for (int i = 0; i < getAmountProp(); ++i)
		{
			cout << " " << setw(33) << setfill('-') << " " << endl;
			cout << "|\t    Property " << i + 1 << "\t\t |" << endl;
			cout << " " << setw(33) << setfill('-') << " " << endl;
			properties[i]->printPropInfo();
			cout << " " << setw(33) << setfill('-') << " " << endl;
			cout << endl;
		}
	}
	void printOwnedProperties(Owner& obj)
	{
		for (int i = 0; i < obj.getAmountProp(); ++i)
		{
			cout << " " << setw(33) << setfill('-') << " " << endl;
			cout << "|\t    Property " << i + 1 << "\t\t |" << endl;
			cout << " " << setw(33) << setfill('-') << " " << endl;
			obj.properties[i]->printPropInfo();
			cout << " " << setw(33) << setfill('-') << " " << endl;
			cout << endl;
		}
	}
	int getBalance() { return wallet; }
	void insertProp(Property& obj) { properties.push_back(obj.getAddress()); }
};

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	Property apart0("Test0", 3000);
	Property apart1("Test1", 5000);
	Property apart2("Test2", 5600);

	//apart0.printPropInfo();

	Owner vasa("Vasanovich", "Vasa", 8000);
	Owner anton("Lukimov", "Anatoly", 4000);

	anton.insertProp(apart2);
	vasa.insertProp(apart0);
	vasa.insertProp(apart1);

	anton.buyProp(vasa);

	cout <<"Properties owned by Vasanovich Vasa"<< endl;		      //итоги сделки для Васлилия
	cout << "Vasanovich Vasa balance: " << vasa.getBalance() << endl; //итоги сделки для Васлилия
	vasa.printOwnedProperties();

	return 0;
}