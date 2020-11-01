#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::string;

//Maximum number of array
#define MAX_MEMBERS 100
#define MAX_ITEMS 100
#define MAX_CATEGORIES 100

//Global varibales

unsigned int last_mem;
unsigned int last_itm;
unsigned int last_category;
string ctgory[MAX_CATEGORIES];

//Functions

void clear_screen(); //Clears screen for another menu
void pause(); //Pauses Screen for user to read
char chk_ans(char start, char end); //Checks if the entered option is right
bool chk_ans(); //checks if entered yes or no
void feedback(unsigned int user_id); //FeedBack for users
void buy_item(unsigned int buyer_id);
void print_report(); 
int login();
void search();
char startPage();
void AccountHome(unsigned int index,char& menu);

//Structs

struct date
{
    unsigned short int day,month,year;

    void read()
    {
        cout << "Enter Today's Date in the following format :\n";
        cout << "DD MM YYYY\n";
        
        cin >> day >> month >> year;
        cin.ignore();

        while (day < 1 || day > 31)
        {
            cout << "Please enter the right day\n";
            cin >> day;
            cin.ignore();
        }

        while(month > 12 || month < 0)
        {
            cout << "Please enter the right month\n";
            cin >> month;
            cin.ignore();
        }
        
        
        while(year < 2020)
        {
            cout << "Please enter the right year\n";
            cin >> year;
            cin.ignore();
        }
    }

    void print()
    {
        cout << day << '/' << month << '/' << year << '\n';
    }

    float to_days()
    {
        return (year * 365.25f) + (month * 30) + day; 
    }
};

struct Item
{
    unsigned int price=0, owner_id=0, category_id=0;
    date publish_date = {}, sell_date = {};

    bool avail=0;

    void create(unsigned int id)
    {
        cout << "Category : ";
        
        string temp;
        cin >> temp;
        cin.ignore();

        bool found=0;

        for (unsigned int  i = 0; i < MAX_CATEGORIES; i++)
        {
            if(temp == ctgory[i])
            {
                category_id = i;
                found=1;
                break;
            }
        }

        if(!found)
        {
            ctgory[last_category] = temp;
            category_id = last_category++;
        }

        publish_date.read();

        cout << "Price : ";
        cin >> price;
        cin.ignore();

        owner_id=id;

        avail=1;
    }

    void view(unsigned int id)
    {
        if(avail)
        {
            cout << "-----------------------\n";
            cout << "Category : " << ctgory[category_id] << '\n';
            cout << "Price : " << price << '\n';
            cout << "Date Added : "; 
            publish_date.print();
            cout << "Item ID : " << id << '\n';
        }
    }
}itms[MAX_ITEMS];

struct Request
{
    unsigned int buyer_id=0,price_offered=0,itm_id=0;

    void create(unsigned int buyer, unsigned int price, unsigned int itm)
    {
        buyer_id = buyer;
        price_offered=price;
        itm_id = itm;
    }

    void accept()
    {
        itms[itm_id].avail=0;
        
        itms[itm_id].sell_date.read();
    }

    void view(unsigned int rqust_id)
    {
        cout << "Item Number : " << itm_id +1 << '\n';
        cout << "Category : " << ctgory[itms[itm_id].category_id] << '\n';
        cout << "Price Offered : " << price_offered << '\n';
        cout << "Buyer ID : " << buyer_id << '\n';
        cout << "Request ID : " << rqust_id << '\n';
        cout << ".......................................\n";
    }
};

struct member
{
    unsigned int points=0; 
    string email="", name="", pass="", dlv_adrs="", phone="", feedbacks="";
    
    Request rqust[100];
    unsigned int last_rqust=0;

    bool chk_email(member mem[])
    {
        for (unsigned int i = 0; i < last_mem - 1; i++)
        {
            if(mem[i].email == email)
            {
                cout << "Already used\n";
                return false;
            }
        }
        
        unsigned int at_sign = email.find("@"), dot_sign = email.find(".", at_sign);
        
        if(at_sign != -1 && dot_sign != -1 && at_sign > 0 && dot_sign < email.size() - 1)
        {
            return true;
        }
        else
        {
            cout << "Invalide Email Format\n";
            return false;
        }   
    }

    bool chk_phone()
    {
        for(unsigned int i=0; i < phone.size(); i++)
        {
            if(phone[i] < '0' || phone[i] > '9')
                return false;
        }
        return true;
    }

    void createAccount(member mem[])
    {
        cout << "Please Fill Your Data\n";

        cout << "Name : ";
        getline(cin, name);

        cout << "Delivery Address : ";
        getline(cin, dlv_adrs);

        while (1)
        {
            cout << "Phone Number : ";
            cin >> phone;
            cin.ignore();

            if(chk_phone())
                break;
            else
                cout << "Phone should only contain numbers!\n";
        }
        
        
        while (1)
        {
            cout << "Email : ";
            cin >> email;
            cin.ignore();


            if(chk_email(mem))
                break;
        }

        cout << "password : ";
        getline(cin, pass);

        cout << "Acccount Created Successfully\n";
        pause();
    }

    void list_all_itms(unsigned int mem_index)
    {
        for (unsigned int i = 0; i <= last_itm; i++)
        {
            if(itms[i].owner_id == mem_index)
                itms[i].view(i + 1);
        }
        
    }

    void add_itm(unsigned int id)
    {
        itms[last_itm++].create(id);
    }

    void show_requests()
    {
        cout << "---------All Requests-------------\n";

        for (unsigned int i = 0; i < last_rqust; i++)
        {
            rqust[i].view(i+1);
        }
    }

    void request_menu()
    {
        cout << "1- Accept a request\n";
        cout << "2- Refuse a request\n";
        cout << "3- Return\n";

        if(chk_ans('1', '3') == '1')
        {
            unsigned int id;
            cout << "Enter request id : ";
            cin >> id;
            cin.ignore();

            while (id > last_rqust)
            {
                cout << "id doesn't exist \n";
                cin >> id;
                cin.ignore();
            }

            rqust[id-1].accept();
          
            feedback(rqust[id-1].buyer_id);
            
            for (unsigned int i = id; i < last_rqust; i++)
            {
                rqust[id + 1] = rqust[id];
            }

            last_rqust--;
        }
            
    }

}mem[MAX_MEMBERS];

//main
int main()
{
    system("color f0");
    int current_mem;
    unsigned short int logins_attemped=0;

    char menu = 's'; //Char in which to identify curreny displayed page
    /*
        StartMenu --> s
        Login Page --> l
        Account Menu --> a
        Create Account --> c
    //*/

    while(1)
    {
        switch (menu)
        {
        case 's':
            switch (startPage())
            {
            case '1':
                menu = 'l';
                break;
            case '2':
                menu = 'c';
                break;
            case '3':
                return 0;
                break;
            }
            clear_screen();
            break;
        
        case 'l':
            current_mem = login();
            
            if(current_mem != -1)
            {
                menu = 'a';
                logins_attemped=0;
                clear_screen();
            }
            else if(++logins_attemped < 3)
            {
                cout << "Email or Password incorrect!\n";
                cout << "You Have " << 3 - logins_attemped << " Attempts Left\n";
            }
            else //Return Home Page
                menu = 's';
            break;
        case 'a':
            AccountHome(current_mem, menu);
            clear_screen();
            break;
        case 'c':
            mem[last_mem++].createAccount(mem);
            menu='s';
            clear_screen();
            break;
        }
    }
    return 0;
}

//Function Definetions
void clear_screen() //clear scrreen by printing new lines
{
    for (unsigned int i = 0; i < 3; i++)
    {
        cout << '\v';
    }
    cout << "----------------------------------------------\n";
}

void pause() // pauses for the user to read
{
    
    cout << "Press Enter to continue...\n";
    getchar();
}

char chk_ans(char start, char end) //Check if answer provided by user is between available options and return which option choosed
{
    string ans;

    while(1)
    {
        cin >> ans;
        cin.ignore();

        if(ans.size() != 1 || ans[0] < start || ans[0] > end)
            cout << "Invalid Option!\n";
        else
            return ans[0];
    }
}

bool chk_ans() //check if answer yes or no
{
    string ans;
    
    while(1)
    {
        cin >> ans;
        cin.ignore();

        ans[0] = tolower(ans[0]);

        if(ans[0] != 'y' && ans[0] != 'n')
            cout << "Invalid Option!\n";
        else if(ans[0] == 'y')
            return true;
        else
            return false;
    }
}

void feedback(unsigned int user_id) //adds feedback message for the user id provided 
{
    if(user_id < last_mem)
    {
        cout << "rate the user number " << user_id +1 << " from 1 to 5 : ";
        
        if(chk_ans('1', '5') >= '3')
        {
            mem[user_id].points++;
        }

        cout << "Do you want to give a comment ? (y/n) : ";
        
        if(chk_ans())
        {
            cout << "Enter Your Comment in one line\n";

            string comment;
            getline(cin, comment);

            mem[user_id].feedbacks += "From User number" + user_id + comment + "\n";
        }
    }
    else
    {
        cout << "User don't exist";
    }
    
}

void buy_item(unsigned int buyer_id)
{
    unsigned int id;

    cout << "Enter Item ID : ";
    cin >> id;
    cin.ignore();

    if(id-1 < last_itm)
    {
        if(itms[id-1].owner_id != buyer_id)
        {
            itms[id-1].view(id);
            cout << "Do you want to offer another price (y/n) : ";
            
            if (chk_ans())
            {
                unsigned int price;

                cout << "Offered Price : ";
                cin >> price;
                cin.ignore();

                mem[itms[id-1].owner_id].rqust[mem[itms[id-1].owner_id].last_rqust++].create(buyer_id, price, id-1);
            }
            else
            {
                mem[itms[id-1].owner_id].rqust[mem[itms[id-1].owner_id].last_rqust++].create(buyer_id, itms[id-1].price, id-1);
            }

            feedback(itms[id-1].owner_id);
        }
        else
        {
            cout << "You Can't Buy you own item\n";
            pause();
        }
        
        
    }
    else
    {
        cout << "ID Not Found !\n";
        pause();
    }

}

void print_report() //Print report of the top category last month
{
    date today;
    today.read();
    
    unsigned int* points = new unsigned int[last_category]();

    //Counting items sold in the last month
    for (unsigned int i = 0; i < last_itm; i++)
    {
        if(!itms[i].avail)
        {
            float diff = today.to_days() - itms[i].sell_date.to_days();

            if(diff <= 30 && diff >= 0)
            {
                points[itms[i].category_id]++;
            }
        }
    }
    
    //getting most sold item index
    int max=-1, index=-1;

    for (unsigned int i = 0; i < last_category; i++)
    {
        if(static_cast<int>(points[i]) >= max)
        {
            max=points[i];
            index=i;
        }
    }
    
    delete[] points;
    points = NULL;

    //print most sold item if existed
    if(index != -1)
    {
        cout << "Category " << ctgory[index] << " is the top selling this month with " << max << " sold items\n";
        pause();
    }

}

int login() //Login Page
{
    string usrname;
    string pass;
    
    cout << "Enter email : ";
    cin >> usrname;
    cin.ignore();

    
    cout << "Enter Password : ";
    cin >> pass;
    cin.ignore();


    for (unsigned int i = 0; i <= last_mem; i++)
    {
        if ( mem[i].email == usrname && mem[i].pass == pass)
        {
            return i;
        }
    }
    
    return -1;
}

void search() //Search in items of marketplace by category or price
{
    cout << "category : ";
    string category;
    cin >> category;
    cin.ignore();

    cout << "Do you want to set limit for price in the search (y/n) : ";

    bool found=0;

    if(chk_ans())
    {
        unsigned int price;

        cout << "enter price limit : ";
        cin >> price;
        cin.ignore();

        for(unsigned int i=0; i <= last_itm; i++)
        {
            if(ctgory[itms[i].category_id] == category && itms[i].price <= price)
            {
                itms[i].view(i + 1);
                found=1;
            }
            
        }
    }
    else
    {
        for(unsigned int i=0; i < last_itm; i++)
        {
            if(ctgory[itms[i].category_id] == category)
            {
                itms[i].view(i + 1);
                found=1;
            }

        }
    }

    if(!found)
            cout << "\t\tNo Results Found!\n";
}

char startPage() //Home Page of the App
{
    cout << "Welcome to the OAS!\n";
    cout << "1- Login\n";
    cout << "2- Create New Account\n";
    cout << "3- Exit\n";

    return chk_ans('1', '3');
}

void AccountHome(unsigned int index,char& menu) //User Menu Page
{
    cout << "Welcome " << mem[index].name << "\n\n";
    cout << "1- Show Marketplace\n";
    cout << "2- Pending Requests(" << mem[index].last_rqust << ")\n";
    cout << "3- Sell Item\n";
    cout << "4- Buy Item\n";
    cout << "5- Search\n";
    cout << "6- Generate Report\n";
    cout << "7- Logout\n";

    char ans;

    ans = chk_ans('1', '7');

    switch (ans)
    {
    case '1':
        cout << "--------------- Marketplace ------------------\n";
        for (unsigned int i = 0; i <= last_mem; i++)
        {
            mem[i].list_all_itms(i);
        }
        pause();
        break;
    case '2':
        mem[index].show_requests();
        mem[index].request_menu();
        break;
    case '3':
        mem[index].add_itm(index);
        break;
    case '4':
        buy_item(index);
        break;
    case '5':
        search();
        pause();
        break;
    case '6':
        print_report();
        break;
    case '7':
        clear_screen();
        menu='s';
        break;
    }
}