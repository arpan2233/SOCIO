#include<iostream>
#include<fstream>
#include<vector>
#include<climits>
#include<unordered_map>
#include<queue>
#include<string>
using namespace std;

class trienode{
    vector<trienode*> v;
    public:
        bool dark;
        char c;
        
        trienode(){
            v.resize(123,NULL);
            dark = false;
        }
        void maketrienode(char c) {
            trienode* temp = new trienode;
            temp->c = c;
            v[c] = temp;
            return;
        }
        trienode* gettrienode(char c) {
            return v[c];
        }
        ~trienode(){
            for(int i = 0; i < 123; i++) {
                delete v[i];
            }
        }
};
void Enter_trie(trienode* root, string inputstring) {
    if(root->gettrienode(inputstring[0]) == NULL) {
        root->maketrienode(inputstring[0]);
    }
    if(inputstring.size() != 1) {
        Enter_trie(root->gettrienode(inputstring[0]), inputstring.substr(1));
    } else {
        root->gettrienode(inputstring[0])->dark = true;
    }
    return;
}
void getallaccounts(trienode* root, string incompleteusername, string s, vector<string> & ans) {
    
    if(root->dark) {
        ans.push_back(s);
    }
    if(incompleteusername.size() != 0) {
        if(root->gettrienode(incompleteusername[0]) == NULL) {
            return;
        }
        s += incompleteusername[0];
        getallaccounts(root->gettrienode(incompleteusername[0]), incompleteusername.substr(1), s, ans);
    } else {
        for(int i = 0 ; i < 123; i++) {
            if(root->gettrienode(i) != NULL) {
                char c1 = i;
                getallaccounts(root->gettrienode(c1), incompleteusername, s + c1, ans);
            }
        }
    }
    return;
}

class user{
    private:
        string username = "";
        string password = "";

    public:
        static bool accountexist(string usernametemp){
            string checkfilename = usernametemp + ".txt";
            fstream file(checkfilename);
            if(file.is_open()){
                return true;
                file.close();
            } else {
                return false;
            }
        }
        static bool validinput(string inspectstring) {
            int n = inspectstring.size();
            for(int i = 0 ; i < n; i++) {
                if((inspectstring[i] >= 48 && inspectstring[i] <= 57) || (inspectstring[i] >= 97 && inspectstring[i] <= 122) || (inspectstring[i] >= 65 && inspectstring[i] <= 90)){
                    continue;
                } else{
                    return false;
                }
            }
            return true;
        }
        bool createaccount(string username, string password) {
            this->username = username;
            this->password = password;
            string filename = username + ".txt";
            ofstream file;
            file.open(filename, ios::app);
            if(!file) {
                return false;
            }
            file << password << endl; 
            file.close();
            fstream file1;
            file1.open("allaccounts.txt", ios::app);
            file1 << username << endl;
            file1.close();
            return true;
        }
        bool checkpassword(string username, string password) {
            string readpassword = "";

            fstream file(username + ".txt");
            getline(file, readpassword);
            file.close();
            if(readpassword == password) return true;
            else return false;
        }
        string getusername(){
            return username;
        }
        static vector<string> getallaccounts(string readfile){
            //read all accounts 
            ifstream file;
            readfile = readfile + ".txt" ;
            file.open(readfile, ios::in);
            string line;
            vector<string> ans;
            while (getline(file, line))
            {
                ans.push_back(line);
            }
            file.close();
            return ans;
        }
};
vector<string> follower(string username) {
    ifstream file;
    file.open(username + ".txt", ios::in);
    vector<string> v; 
    if(file) {
        string line = "";
        bool password = true;
        while (getline(file, line))
        {
            if(password){
                password = false;
                continue;
            }
            if(file.bad()){
                cerr<<"Error reading file"<<endl;
                continue;
            }
            v.push_back(line);
        }
        file.close();
    }
    return v;
}
bool Enter_following(string username, string followedby) {
    string filename = username + ".txt";
    ofstream file;
    file.open(filename, ios::app);
    if(!file){
        return false;
    }
    file << followedby << endl;
    file.close();
    return true;
}
vector<int> Dijkstra_algo(int S, int end, vector<vector< pair<int,int> > > & adj) {
    int n = adj.size();
    vector<int> parent(n, -1);
    vector<int> weights(n, INT_MAX);
    int index = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> p;
    pair<int,int> a;
    a.first = 0;
    a.second = S;
    p.push(a);
    weights[S] = 0;
    vector<bool> visited(n, false); 
    while(!p.empty()) {
        index = p.top().second;
        p.pop();
        if(visited[index]){
            continue;
        }
        int x = adj[index].size();
        for(int i = 0; i < x; i++) {
            if( weights[adj[index][i].first] > weights[index] +adj[index][i].second) {
                weights[adj[index][i].first] = weights[index] +adj[index][i].second;
                a.first = weights[adj[index][i].first];
                a.second = adj[index][i].first;
                p.push(a);
                parent[adj[index][i].first] = index;
            }
        }
        visited[index] = true;
    }
    vector<int> ans;
    while(parent[end] != -1) {
        ans.push_back(end);
        end = parent[end];
    }
    ans.push_back(end);
    return ans;
}
void displaywelcomepage() {
    cout<<"|:|:|-- Welcome To SOCIO --|:|:|"<<endl;
    cout<<"To Create an account -> ENTER 1"<<endl;
    cout<<"To open an exisiting account -> ENTER 2"<<endl;
    cout<<"To see all accounts on SOCIO -> ENTER 3"<<endl;
    cout<<"To EXIT! -> ENTER -1"<<endl;
    return;
}

void displaymenu() {
    cout<<"To see The people you FOLLOW on SOCIO -> ENTER 1"<<endl;
    cout<<"To see all accounts on SOCIO -> ENTER 2"<<endl;
    cout<<"To search/Follow for an existing account -> ENTER 3"<<endl;
    cout<<"To approach a SOCIO user through minimum intermedite users -> ENTER 4"<<endl;
    cout<<"To EXIT! -> ENTER -1"<<endl;
    return;
}
int tryagain() {
    int choice;
    cout<<"To try again -> ENTER 1"<<endl;
    cout<<"To EXIT -> ENTER -1"<<endl;
    cin>>choice;
    if(choice == 1) {
        cout<<"To EXIT to MENU -> ENTER 1"<<endl;
        cout<<"To directly ENTER your choice -> ENTER 0"<<endl;
        cin>>choice;
    }
    return choice;
}

int main(){
    
    displaywelcomepage();

    user* actualuser = new user;
    trienode* root = new trienode;
    vector<string> all_accounts = user::getallaccounts("allaccounts");
    int all_accounts_size = all_accounts.size();

    int choice = 0;
    bool appopened = false, newaccount = false;
    string username = "";

    while(choice != -1) {
        
        cout<<"CHOICE: ";
        cin>>choice;
        //various choices at welcomepage
        if(choice == 1) {
            cout<<"Username must conist of *only* english alphabets or/and digits from 0 to 9"<<endl;
            cin>>username;
            if(user::validinput(username)) {
               if(user::accountexist(username)) {
                    cout<<"OOPS! this username is already taken"<<endl;
                } else {
                    cout<<"Enter the password"<<endl;
                    cout<<"Password must contain *only* english alphabets or/and digits from 0 to 9"<<endl;
                    string password;
                    cin>>password;
                    if(!user::validinput(password)) {
                        cout<<"REMEMBER : The password must contain only english alphabets Or/and digits from 0 to 9"<<endl;
                    } else {
                        //ACCOUNT CREATION
                        bool accountcreated = actualuser->createaccount(username, password);
                        if(!accountcreated) {
                            cout<<"There was an error while creating our account"<<endl;
                        } else {
                            cout<<"Account was successfully created"<<endl;
                            appopened = true;
                            newaccount = true;
                            break;
                        }
                    }
                }
            }
        } else if(choice == 2){
            cout<<"Username must conist of english alphabets or/and digits from 0 to 9"<<endl;
            cin>>username;
            if(user::validinput(username)) {
                if(!user::accountexist(username)) {
                    cout<<"OOPS! Account not found! \n Please try again :) \n";
                } else {
                    cout<<"Enter the password"<<endl;
                    cout<<"Password must contain *only* english alphabets or/and digits from 0 to 9"<<endl;
                    string password;
                    cin>>password;
                    if(user::validinput(username)) {
                        if(actualuser->checkpassword(username, password)) {
                            cout<<"Hello '"<< username <<"'"<<endl;
                            appopened = true;
                            break;
                        } else {
                            cout<<"Password is incorrect for the provided username "<<endl;
                        }
                    }
                }
            }
        } else if(choice == 3){
            all_accounts = user::getallaccounts("allaccounts");
            all_accounts_size = all_accounts.size();
            if(all_accounts_size == 0){
                cout<<"There are no accounts right now on SOCIO"<<endl;
                cout<<"You can be the first one"<<endl;
                cout<<"Sign up today"<<endl;
            }
            for(int i = 0; i < all_accounts_size; i++) {
                cout<<i+1 << ": " << all_accounts[i]<<endl;
            }
            cout<<"To EXIT to welcome page -> Enter 1"<<endl;
            cout<<"To go directly ENTER your CHOICE -> Enter 2"<<endl;
            cout<<"To EXIT! -> Enter -1"<<endl;
            cin>>choice;
            if(choice == 1) {
                displaywelcomepage();
            }
            continue;
        } else if(choice == -1){
            break;
        } else {
            cout<<"Please Enter a Valid CHOICE";
        }
        choice = tryagain();
        if(choice == 1){
            displaywelcomepage();
        }
    }
    if(newaccount) {
        all_accounts.push_back(actualuser->getusername());
        all_accounts_size++;
    }

    // void displaymenu() {
    //     cout<<"To see The Followers list -> ENTER 1"<<endl;
    //     cout<<"To see all accounts on SOCIO -> ENTER 2"<<endl;
    //     cout<<"To search for an existing account -> ENTER 3"<<endl;
    //     cout<<"To approach a SOCIO user through minimum intermedite users -> ENTER 4"<<endl;
    //     cout<<"To EXIT! -> ENTER -1"<<endl;
    //     return;
    // }
    if(appopened) {
        displaymenu();
        choice = 0;
        while(choice != -1) {
            cout<<"CHOICE: ";
            cin>>choice;
            if(choice == 1) {
                vector<string> v = follower(username);
                int num_followers = v.size();
                if(num_followers == 0) {
                    cout<<"You don't follow anyone right now"<<endl;
                } else { 
                    cout<<"Sure! \nYou FOLLOW these people on SOCIO."<<endl;
                    for(int i = 0; i < num_followers; i++) {
                        cout<<i+1<<": "<<v[i]<<endl;
                    }
                }
                cout<<"To go back to menu -> Enter 1"<<endl;
                cout<<"To go directly ENTER your CHOICE -> Enter 2"<<endl;
                cout<<"To EXIT! -> Enter -1"<<endl;
                cin>>choice;
                if(choice == 1) {
                    displaymenu();
                }
                continue;
            } else if(choice == 2){
                for(int i = 0; i < all_accounts_size; i++) {
                    cout<<i+1 << ": " << all_accounts[i]<<endl;
                }
                cout<<"To go back to menu -> Enter 1"<<endl;
                cout<<"To go directly ENTER your CHOICE -> Enter 2"<<endl;
                cout<<"To EXIT! -> Enter -1"<<endl;
                cin>>choice;
                if(choice == 1) {
                    displaymenu();
                }
                continue;
            }else if(choice == 3 || choice == 4) {
                //BUILDING TRIE
                for(int i = 0; i < all_accounts_size; i++) {
                    Enter_trie(root, all_accounts[i]);
                }
                cout<<"Enter the username you want to search"<<endl;
                cout<<"**Don't worry if you don't remember the full username ";
                cout<<"Just Enter the starting few characters correct and we got you**"<<endl;
                if(choice == 3) {
                    
                    string findusername = "";
                    cin>>findusername;
                    bool usernamefound = user::accountexist(findusername);
                    if(!usernamefound){
                        vector<string> ans;
                        string s ="";
                        getallaccounts(root, findusername, s, ans);
                        int ans_size = ans.size();
                        if(ans_size == 0) {
                            cout<<"There are no matching usernames"<<endl;
                        } else {
                            cout<<"These are matching USERNAMES found"<<endl;
                            for(int i = 0; i < ans_size; i++) {
                                cout<<i+1<<": "<<ans[i]<<endl;
                            }
                            cout<<"To FOLLOW any user -> ENTER 1"<<endl;
                            cout<<"To EXIT -> ENTER -1"<<endl;
                            int internalchoice = 0;
                            cin>>internalchoice;
                            if(internalchoice == 1){
                                cout<<"Enter the username whome You want to FOLLOW"<<endl;
                                string followuser = "";
                                cin>>followuser;
                                // REQUEST TO FOLLOW
                                
                                bool request = Enter_following(username, followuser); 
                                if(request){
                                    cout<<"You are now FOLLOWING "<<followuser<<endl;
                                } else {
                                    cout<<"There was an ERROR while sending request \n PLEASE try again"<<endl;
                                }
                                cout<<"To display Menu -> Enter 1"<<endl;
                                cout<<"To directly Enter your choice -> ENTER 2"<<endl;
                                cout<<"To EXIT -> ENTER -1"<<endl;
                                if(choice == 1){
                                    displaymenu();
                                }
                                continue;
                            }
                        }
                            cout<<"To display Menu -> Enter 1"<<endl;
                            cout<<"To directly Enter your choice -> ENTER 2"<<endl;
                            cout<<"To EXIT -> ENTER -1"<<endl;
                            if(choice == 1){
                                displaymenu();
                            }
                            continue;
                    } else {
                        cout<<"To FOLLOW -> ENTER 1"<<endl;
                        cout<<"TO EXIT! -> ENTER -1"<<endl;
                        cin>>choice;
                        if(choice == 1) {
                            //REQUEST TO FOLLOW
                            bool request = Enter_following(username,findusername); 
                            if(request){
                                cout<<"You are now FOLLOWING "<<findusername<<endl;
                            } else {
                                cout<<"There was an ERROR while sending request \n PLEASE try again"<<endl;
                            }
                           
                        }
                        cout<<"To display Menu -> Enter 1"<<endl;
                        cout<<"To directly Enter your choice -> ENTER 2"<<endl;
                        cout<<"To EXIT -> ENTER -1"<<endl;
                        if(choice == 1){
                            displaymenu();
                        }
                        continue;
                    }
                } else {
                    
                    string findusername = "";
                    cin>>findusername;
                    bool usernamefound = user::accountexist(findusername);
                    if(!usernamefound) {
                        vector<string> ans;
                        string s = "";
                        getallaccounts(root, findusername, s, ans);
                        int n = ans.size();
                        if(n == 0) {
                            cout<<"There are no matching accounts on SOCIO"<<endl;
                            cout<<"To display Menu -> Enter 1"<<endl;
                            cout<<"To directly Enter your choice -> ENTER 2"<<endl;
                            cout<<"To EXIT -> ENTER -1"<<endl;
                            if(choice == 1){
                                displaymenu();
                            }
                            continue;
                        } else {
                            cout<<"Select among the following usernames"<<endl;
                            for(int i = 0 ; i < n; i++) {
                                cout<<i+1<<": "<<ans[i]<<endl;
                            }
                            cin>>findusername;
                        }
                    }
                    unordered_map<string, int> map;
                    for(int i = 0; i < all_accounts_size; i++) {
                        map[all_accounts[i]] = i;
                    }
                    vector<vector<string>> v(all_accounts_size);
                    for(int i = 0; i < all_accounts_size; i++) {
                        string getfollower = all_accounts[i];
                        v[i] = follower(getfollower);
                    }
                    pair<int,int> p;
                    p.second = 1;
                    vector<vector<pair<int,int>>> adj_list(all_accounts_size);
                    for(int i = 0; i < all_accounts_size; i++) {
                        
                        int n = v[i].size();
                        for(int j = 0; j < n; j++) {
                            p.first = map[v[i][j]];
                            adj_list[i].push_back(p);
                        }
                    }
                    int start = map[username], end = map[findusername];
                    vector<int> path = Dijkstra_algo(start, end, adj_list);
                    cout<<"'"<<username<<"' can approach '"<<findusername<<"' through following users"<<endl;
                    int sizeofpath = path.size();
                    for(int i = sizeofpath - 1; i >= 0; i--) {
                        cout<<all_accounts[path[i]];
                        if(i != 0) cout<<"->";
                    }
                    cout<<endl;
                    cout<<"To EXIT to menu -> Enter 1"<<endl;
                    cout<<"To go directly ENTER your CHOICE -> Enter 2"<<endl;
                    cout<<"To EXIT! -> Enter -1"<<endl;
                    cin>>choice;
                    if(choice == 1) {
                        displaymenu();
                    }
                    continue;
                }

            } else if(choice == -1) {
                break;
            } else {
                cout<<"Please ENTER a VALID choice"<<endl;
                choice = tryagain();
                if(choice == 1) {
                    displaymenu();
                }
            }
            
        }
    }
    delete actualuser;
    delete root;
    return 0;
}