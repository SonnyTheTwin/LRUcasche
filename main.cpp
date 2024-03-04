#include <iostream>
#include <fstream>


/*

Uppgift:
Skapa en FIL med 100000 random hockeyspelare. HockeyPlayer = id (löpnummer), namn, jersey, teamName
Skapa en klass LRUCache<HockeyPlayer> 
- plats för 10 st players

Vid anrop till getPlayer så kollar man först i cachen
Om träff - lägg längst fram - och returnera
Om miss - läs från fil, och stoppa in (först) - ta bort sista
class LRUCache : 
     Player *GetPlayer(int id)  - returnera Player eller NULL om inte finns*/



class HockeyPlayer
{
private:
    int shirt_number;
    std::string name;
    std::string team;

public:

    HockeyPlayer(){}; //default constructor

    HockeyPlayer(int shirt_number, std::string name, std::string team)
    {
        this->name = name; 
        this->shirt_number = shirt_number; 
        this->team = team;
    }
    ~HockeyPlayer(){}; // deconstructor



    //getters
    int getHockeyPlayerId()
    {
        return this->shirt_number;
    };
    std::string getHockeyPlayerName()
    {
        return this->name;
    };
    std::string getHockeyPlayerTeam()
    {
        return this->team;
    };
    //skapa setters här om vi vill manipulera värden(behövs inte just nu då constructer används)
};

class Filehandler
{
private:
    int number_of_entries = 100000; // ändra till 100 000 om de ska stämma med uppgiften
    std::ofstream ofile;
    std::ifstream ifile;
public:
    Filehandler(){};
    ~Filehandler(){};

    void CreateFile()
    {
        ofile.open("hockyplayer.txt"); // opens and creates file

        if (!ofile.is_open()) // check if file is open
        {
            std::cout << "File couldnt be opened" << std::endl;
        }
        else
        {
            for (int i = 0; i < number_of_entries; i++) // loop för att fylla .txt filen
                {
                    fillFile(i); // calls memberfunction to fill file 
                }
        }
        ofile.close(); // closes file when we done reading/or/writing
    };

    void fillFile(int i)
    {
        ofile.open("hockyplayer.txt", std::fstream::app); // opens file in "append-mode", so all information is appended to the existing information
        
        if (!ofile.is_open()) // closes file when we done reading/or/writing
        {
           std::cout << "File couldnt be opened" << std::endl;
        }
        
                HockeyPlayer new_player(i, "TestNamn", "Testlag"); // skapar en spelare som vi senare stoppar in(här skriver vi ändringar om vi vill själva kunna välja informationen för varje individuell spelare)
                ofile << i << " " << "Testnamn" << " " << "TestLag" <<"\n"; // stoppar in informationen(Här kan vi ändra till getters från Hockeyplayer classen, just nu har vi skrivit in det manuellt)
                ofile.close(); // stänger fil efter vi klar med den
    };

    HockeyPlayer *getPlayerFile(int key)
    {
        int id;
        std::string name;
        std::string team;
        ifile.open("hockyplayer.txt");

        if (!ifile.is_open()) // check if file is open
        {
            std::cout << "File couldnt be opened" << std::endl;
        }
        
        while (!ifile.eof()) // while we havent reached the end of the file(eof = end of file)
        {
            ifile >> id >> name >> team; // for each itteration we extract the information on each line in the .txt file
            if (id == key) // we found what we were looking for! 
            {
                HockeyPlayer *player_from_file = new HockeyPlayer(id,name,team); // allocated memory to created object 
                ifile.close(); // closes file when we done reading/or/writing
                return player_from_file;
            }
        }
        
    ifile.close();  // closes file when we done reading/or/writing
    return nullptr; // if we dont find what we looking for we return a Null pointer
    }

};


class LRUCache
{
private:
    int array_size = 10;
    HockeyPlayer array_cache[10];
public:

    LRUCache(){};
    ~LRUCache(){};

    void printCache()
    {
        for (HockeyPlayer x : array_cache) // for each loop
        {
            std::cout << "In Array we find\n id: " << x.getHockeyPlayerId() << ", name: " << x.getHockeyPlayerName() << ", team: " << x.getHockeyPlayerTeam()  << "\n" << std::endl;
        }
    }


    void addToCache(HockeyPlayer *player_from_file)
    {
        HockeyPlayer temp = *player_from_file; // derefference our pointer 
        for (int i = array_size-1; i > 0; i--) // loops through array
        {
            array_cache[i] = array_cache[i-1];
        }
        array_cache[0] = temp; // places our new element in front!
    }

    HockeyPlayer *getPlayerCache(int key)
    {
        for (int i = 0; i < array_size; i++)
        {
            if (key == array_cache[i].getHockeyPlayerId())
            { 
                // normal switch, tho the internal order logic is screwed, if we find a hit we switch places with [0] so its in front  
                // forloop to make correct order?  
                HockeyPlayer temp = array_cache[i];
                for (int a = i; a > 0; a--)
                {
                    array_cache[a] = array_cache[a-1];
                }
                array_cache[0] = temp;

                HockeyPlayer* player_from_cache = &temp; // creates a pointer for us to return
                return player_from_cache;
            }
        }    
    return nullptr;      
    }    
};  


//Here we can design on how to present information to user
void getPlayer(int key, LRUCache& Cache, Filehandler& file) // function that handles both LRUcache and Filehandler
{
    HockeyPlayer *searched_player = Cache.getPlayerCache(key);
    if (searched_player == nullptr) // if we dont get a hit from looking in the cache
    {
        std::cout << "spelare inte hittad i cachen!" << std::endl; 
        searched_player = file.getPlayerFile(key); // fetches player from .txt file
        if (searched_player == nullptr) // if we dont get a hit in the  .txt file
        {
            std::cout << "Du sökte på en spelare som inte finns!" << std::endl; // ändra
            return;
        }
        else if (searched_player != nullptr) // if we get a hit in the .txt filen
        {
            std::cout << "spelare tillagd i cachen från fil" << std::endl;
            Cache.addToCache(searched_player);
            return;
        }  
    }
    else if (searched_player != nullptr) // if we get a hit in the cachen
    {
        std::cout << "spelare hittat i cachen!" << std::endl; // skriv ut informationen på något snyggt sätt
        return;
    }
    delete searched_player; // deletes the element we allocated memory for in filehandler
}









int main()
{


    Filehandler file;
    file.CreateFile();
    LRUCache Cache;

    int key = 2;


    /*
    int user_input;

    std::cout << "Welcome to HPD\n" 
                << "the Hockey Player Database\n\n"
                << "options:\n"
                << "1. Search Database\n"
                << "2. Exit Program\n"
                << "Input: ";
    std::cin >> user_input;
    std::cout << std::endl;

    if (user_input == 1) // Search Database
    {
        std::cout << "1. Search Database\n"
                  << "Input Id of player you looking for: ";
        std::cin >> key;
        std::cout << std::endl;
        getPlayer(key,Cache,file);
    }
    else if (user_input == 2) // Exit program
    {
        // make exitbool = true
    }
    else // invalid input
    {
        std::cout << ">>ERROR<< Invalid input!" << std::endl; // invalid input, användare flyttas tillbaka till huvudmeny
    }
    */
    
    
    
    // TEST // 
    std::cout << std::endl;
    getPlayer(1,Cache,file);
    std::cout << std::endl;
    getPlayer(2,Cache,file);
    std::cout << std::endl;
    getPlayer(3,Cache,file);
    std::cout << std::endl;
    getPlayer(4,Cache,file);
    std::cout << std::endl;
    getPlayer(5,Cache,file);
    std::cout << std::endl;
    getPlayer(6,Cache,file);
    std::cout << std::endl;
    getPlayer(7,Cache,file);
    std::cout << std::endl;
    getPlayer(8,Cache,file);
    std::cout << std::endl;
    getPlayer(9,Cache,file);
    std::cout << std::endl;
    getPlayer(10,Cache,file);
    std::cout << std::endl;
    getPlayer(1,Cache,file);
    std::cout << std::endl;
    getPlayer(8,Cache,file);
    std::cout << std::endl;
    getPlayer(7,Cache,file);
    std::cout << std::endl;
    getPlayer(6,Cache,file);
    std::cout << std::endl;
    
    Cache.printCache();

    getPlayer(33,Cache,file);

    Cache.printCache();



    

    return 0;
}