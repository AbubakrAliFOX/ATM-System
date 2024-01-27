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
void NormalWithdraw();
void ShowInvalidOptionScreen(void (*MainMenuFunc)());
bool IsAmountMultipleOfFiveAndLessThanFifty(int Amount);
void ValidateAmount(string TransactionType, float &Amount);
void ValidateAmount(string TransactionType, float &Amount, float QuickWithDrawOptions[8], int Choice);
////////////////////////

void WithdrawFromAccount(string TransactionType, float Amount)
{
    vector<Client> vAllClients = GetClientsFromFile();
    Client CurrentClient = LogedInClient;

    string Msg;

    for (Client &u : vAllClients)
    {
        if (u.AccountNumber == CurrentClient.AccountNumber)
        {
            CurrentClient = u;
            if (TransactionType == "Withdrawal")
            {
                CurrentClient.AccountBalance -= Amount;
            }

            if (TransactionType == "Deposit")
            {
                CurrentClient.AccountBalance += Amount;
            }

            char Answer = ReadChar("Are you sure you want to complete this transaction? (y / n)");
            if (Answer == 'Y' || Answer == 'y')
            {
                u = CurrentClient;
                OverwriteCutomersToFile(vAllClients);
                LogedInClient = u; // To make loggedIn client same as in the file.
                cout << "\n\n New Balance is:" << LogedInClient.AccountBalance << "\n\n";
            }
            break;
        }
    }
}

void QuickWithdraw()
{
    float QuickWithDrawOptions[8] = {50, 100, 200, 500, 1000, 2000, 5000, 10000};

    int Answer;
    cin >> Answer;

    float Amount = QuickWithDrawOptions[Answer - 1];

    if (Answer >= 1 && Answer <= 8)
    {
        ValidateAmount("Withdrawal", Amount, QuickWithDrawOptions, Answer);
        WithdrawFromAccount("Withdrawal", Amount);
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

bool IsAmountMultipleOfFiveAndLessThanFifty(int Amount)
{
    return Amount % 5 == 0 && Amount >= 50;
}

// void ValidateAmount(int &Amount)
// {
//     while (!(IsAmountMultipleOfFiveAndLessThanFifty(Amount)))
//     {
//         cout << "\nWithdrawal amount must be multiples of 5 and more than 50 !!!" << endl;
//         cout << "\nHow much you like to withdraw?" << endl;
//         cin >> Amount;
//     }
// }

void ValidateAmount(string TransactionType, float &Amount)
{

    if (TransactionType == "Withdrawal")
    {
        while ((Amount > LogedInClient.AccountBalance || !(IsAmountMultipleOfFiveAndLessThanFifty((int)Amount))))
        {
            if (!(IsAmountMultipleOfFiveAndLessThanFifty(Amount)))
            {
                cout << "\nWithdrawal amount must be multiples of 5 and more than 50 !!!" << endl;
                cout << "\nHow much you like to withdraw?" << endl;
                cin >> Amount;
            }
            else
            {
                cout << "\nThe withdrawal amount exceeds the balance, you can withdraw up to " << LogedInClient.AccountBalance << endl;
                cout << "\nHow much you like to withdraw?" << endl;
                cin >> Amount;
            }
        }
    }

    if (TransactionType == "Deposit")
    {
        while (Amount <= 0)
        {
            cout << "\nPlease enter a positive amount:" << endl;
            cin >> Amount;
        }
    }
}

void ValidateAmount(string TransactionType, float &Amount, float QuickWithDrawOptions[8], int Choice)
{
    while (Amount > LogedInClient.AccountBalance)
    {
        cout << "\nThe withdrawal amount exceeds the balance, you can withdraw up to " << LogedInClient.AccountBalance << endl;
        cout << "\nHow much you like to withdraw? (1-8)" << endl;
        cin >> Choice;
        Amount = QuickWithDrawOptions[Choice - 1];
    }
}

void NormalWithdraw()
{
    float Amount;
    cin >> Amount;

    ValidateAmount("Withdrawal", Amount);
    WithdrawFromAccount("Withdrawal", Amount);
}

void ShowNormalWithdrawScreen()
{
    system("cls");
    cout << "===========================================================================\n";
    cout << "                                 Normal Withdraw                                 \n";
    cout << "===========================================================================\n\n";

    cout << "\n Your Balance is " << LogedInClient.AccountBalance << endl;
    cout << "\n How much you like to withdraw? (Enter multiples of 5 and more than 50) \n";

    NormalWithdraw();
}

void Deposit()
{
    float Amount;
    cin >> Amount;

    ValidateAmount("Deposit", Amount);
    WithdrawFromAccount("Deposit", Amount);
}

void ShowDepositScreen()
{
    system("cls");
    cout << "===========================================================================\n";
    cout << "                                 Deposit                                 \n";
    cout << "===========================================================================\n\n";

    cout << "\n How much you like to deposit?\n";

    Deposit();
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
        ShowNormalWithdrawScreen();
        GoBackToMainMenue();
        break;
    case '3':
        system("cls");
        ShowDepositScreen();
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
