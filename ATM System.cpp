#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

const string FileName = "Bank.txt";

struct Client
{
    string Name;
    string AccountNumber, PinCode, Phone;
    float AccountBalance;
};

Client LogedInClient;

char ReadChar(string Msg)
{
    char charachter;
    cout << Msg << endl;
    cin >> charachter;

    return charachter;
}

string ReadString(string Msg)
{
    string Str;
    cout << Msg << "\n";
    cin >> Str;
    return Str;
}

float ReadFloat(string Msg)
{
    float Number;
    cout << Msg << "\n";
    cin >> Number;
    return Number;
}

vector<string> split(string Str, string Delim)
{

    short pos = 0;
    string sWord; // define a string variable  // use find() function to get the position of the delimiters
    short Counter = 0;
    vector<string> vWords;
    while ((pos = Str.find(Delim)) != std::string::npos)
    {
        sWord = Str.substr(0, pos); // store the word
        if (sWord != "")
        {
            vWords.push_back(sWord);
            Counter++;
        }
        Str.erase(0, pos + Delim.length()); /* erase() until positon and move to next word. */
    }
    if (Str != "")
    {
        vWords.push_back(Str); // it print last word of the string.
        Counter++;
    }

    return vWords;
}

Client ConvertLineToRecord(string DataLine, string Seperator)
{
    Client ClientData;
    vector<string> vData = split(DataLine, Seperator);

    ClientData.Name = vData[0];
    ClientData.AccountNumber = vData[1];
    ClientData.PinCode = vData[2];
    ClientData.Phone = vData[3];
    ClientData.AccountBalance = stof(vData[4]);

    return ClientData;
}

vector<Client> GetClientsFromFile()
{
    fstream MyFile;
    vector<Client> vAllClientsRecords;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            Client CurrentClient = ConvertLineToRecord(Line, "##");
            vAllClientsRecords.push_back(CurrentClient);
        }

        MyFile.close();
    }

    return vAllClientsRecords;
}

bool FindClient(string AccountNumber, string PinCode, Client &CurrentClient)
{
    vector<Client> vAllClients = GetClientsFromFile();

    for (Client &u : vAllClients)
    {
        if (AccountNumber == u.AccountNumber && PinCode == u.PinCode)
        {
            CurrentClient = u;
            return true;
        }
    }

    return false;
}

string ConvertRecordToLine(Client ClientData, string Seperator)
{
    string ClientRecord = "";
    ClientRecord += ClientData.Name + Seperator;
    ClientRecord += ClientData.AccountNumber + Seperator;
    ClientRecord += ClientData.PinCode + Seperator;
    ClientRecord += ClientData.Phone + Seperator;
    ClientRecord += to_string(ClientData.AccountBalance);
    return ClientRecord;
}

void OverwriteCutomersToFile(vector<Client> vClients)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        for (Client &u : vClients)
        {
            MyFile << ConvertRecordToLine(u, "##") << "\n";
        }

        MyFile.close();
    }
}

// Declare function for scope problem

void LogInScreen();
void GoBackToMainMenue();
void ShowQuickWithdrawScreen();

void ShowInvalidOptionScreen(void (*MainMenuFunc)());

////////////////////////

void WithdrawFromAccount(string TransactionType, float Amount)
{
    vector<Client> vAllClients = GetClientsFromFile();
    
    string Msg;

    for (Client &u : vAllClients)
    {
        if (u.AccountNumber == LogedInClient.AccountNumber)
        {
            LogedInClient = u;
            if (TransactionType == "Withdrawal")
            {
                while (Amount > LogedInClient.AccountBalance)
                {
                    cout << "The withdrawal amount exceeds the balance, you can withdraw up to " << LogedInClient.AccountBalance << endl;
                    Msg = "Please Enter " + TransactionType + " Amount: ";
                    Amount = ReadFloat(Msg);
                }
                LogedInClient.AccountBalance -= Amount;
            }
            else
            {
                LogedInClient.AccountBalance += Amount;
            }

            u = LogedInClient;
            LogedInClient = u;
            break;
        }
    }

    char Answer = ReadChar("Are you sure you want to complete this transaction? (y / n)");

    if (Answer == 'Y' || Answer == 'y')
    {
        OverwriteCutomersToFile(vAllClients);
    }

    cout << "\n\n New Balance is:" << LogedInClient.AccountBalance << "\n\n";
}

void QuickWithdraw()
{
    float QuickWithDrawOptions[8] = {50, 100, 200, 500, 1000, 2000, 5000, 10000};

    int Answer;
    cin >> Answer;

    if (Answer >= 1 && Answer <= 8)
    {
        WithdrawFromAccount("Withdrawal", (float)QuickWithDrawOptions[Answer - 1]);
    }
    else
    {
        ShowInvalidOptionScreen(ShowQuickWithdrawScreen);
    }
}

void ShowQuickWithdrawScreen()
{
    system("cls");
    cout << "===========================================================================\n";
    cout << "                                 Quick Withdraw                                 \n";
    cout << "===========================================================================\n\n";
    cout << "        [1] 50                \n";
    cout << "        [2] 100                  \n";
    cout << "        [3] 200                  \n";
    cout << "        [4] 500              \n";
    cout << "        [5] 1000                 \n";
    cout << "        [6] 2000                 \n";
    cout << "        [7] 5000                 \n";
    cout << "        [8] 10000                 \n";

    cout << "\n Your Balance is " << LogedInClient.AccountBalance << endl;
    cout << "\n How much you like to withdraw? (1-8) \n";

    QuickWithdraw();
}

void CheckBalance()
{
    cout << "\n\nYour total balance is " << LogedInClient.AccountBalance << endl;
}

void AppOptions()
{
    char Answer;
    cin >> Answer;

    switch (Answer)
    {
    case '1':
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenue();
        break;
    case '2':
        system("cls");
        // CreateCustomers();
        GoBackToMainMenue();
        break;
    case '3':
        system("cls");
        // DeleteCustomer();
        GoBackToMainMenue();
        break;
    case '4':
        system("cls");
        CheckBalance();
        GoBackToMainMenue();
        break;
    case '5':
        system("cls");
        LogInScreen();
        break;
    default:
        cout << "Invalid Input!!!" << endl;
        GoBackToMainMenue();
        break;
    }
}

void ShowMainMenue()
{

    system("cls");

    cout << "===========================================================================\n";
    cout << "                                 Main Menu                                 \n";
    cout << "===========================================================================\n\n";
    cout << "        [1] Quick Withdraw.               \n";
    cout << "        [2] Normanl Withdraw.                 \n";
    cout << "        [3] Deposit.                  \n";
    cout << "        [4] Check Balance.             \n";
    cout << "        [5] Logout.                    \n";

    cout << "\n What would you like to do? (1-5) \n";

    AppOptions();
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menu...\n\n";
    system("pause>0");
    ShowMainMenue();
}

void ShowInvalidOptionScreen(void (*MainMenuFunc)())
{
    system("cls");
    cout << "\n\nInvalid Option !!!!!!\n\n";
    cout << "\n\nPress any key to go back ...\n\n";
    system("pause>0");
    MainMenuFunc();
}

void LogIn()
{
    string AccountNumber = ReadString("Please Enter Account Number: ");
    string PinCode = ReadString("Please Enter PinCode: ");

    while (!FindClient(AccountNumber, PinCode, LogedInClient))
    {
        system("cls");
        cout << "Invalid Username/PinCode!" << endl;
        AccountNumber = ReadString("Please Enter Account Number: ");
        PinCode = ReadString("Please Enter PinCode: ");
    }

    system("cls");
    ShowMainMenue();
}

void LogInScreen()
{
    cout << "------------------------------------------------------------------------------\n";
    cout << "                                Log In Screen                                 \n";
    cout << "------------------------------------------------------------------------------\n\n";
    LogIn();
}

int main()
{
    LogInScreen();
}
