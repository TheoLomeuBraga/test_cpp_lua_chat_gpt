#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "lua/lua.hpp"
#include <sstream>

bool isNumber(const std::string& str) {
    std::istringstream iss(str);
    double num;
    iss >> std::noskipws >> num;
    return iss.eof() && !iss.fail();
}

float stringToFloat(const std::string& str) {
    std::istringstream iss(str);
    float result;
    iss >> result;
    return result;
}

void test_is_number(){
    std::cout << "1 " << isNumber("1") << std::endl;
    std::cout << "a " << isNumber("a") << std::endl;
    std::cout << "1.5 " << isNumber("1.5") << std::endl;
}

class Table {
public:
    std::unordered_map<std::string, float> m_floatMap;
    std::unordered_map<std::string, std::string> m_stringMap;
    std::unordered_map<std::string, Table> m_tableMap;

    void setFloat(const std::string& key, float value) {
        m_floatMap[key] = value;
    }

    float getFloat(const std::string& key) {
        return m_floatMap[key];
    }

    void setString(const std::string& key, const std::string& value) {
        m_stringMap[key] = value;
    }

    std::string getString(const std::string& key) {
        return m_stringMap[key];
    }

    void setTable(const std::string& key, const Table& table) {
        m_tableMap[key] = table;
    }

    Table getTable(const std::string& key) {
        return m_tableMap[key];
    }


    
};

void test_table(){
    Table myTable;
    myTable.setFloat("age", 42);
    myTable.setString("name", "John");

    Table subTable;
    subTable.setFloat("x", 1);
    subTable.setFloat("y", 2);
    myTable.setTable("subTable", subTable);

    std::cout << myTable.getFloat("age") << std::endl;
    std::cout << myTable.getString("name") << std::endl;
    Table retrievedTable = myTable.getTable("subTable");
    std::cout << retrievedTable.getFloat("x") << std::endl;
    std::cout << retrievedTable.getFloat("y") << std::endl;
}

void lua_pushtable(lua_State* L, Table t){
    lua_newtable(L);
    for(std::pair<std::string, float> p : t.m_floatMap){
        if(isNumber(p.first)){
            lua_pushnumber(L, stringToFloat(p.first));
        }else{
            lua_pushstring(L, p.first.c_str());
        }
        lua_pushnumber(L,p.second);
        lua_settable(L, -3);
    }
    for(std::pair<std::string, std::string> p : t.m_stringMap){
        if(isNumber(p.first)){
            lua_pushnumber(L, stringToFloat(p.first));
        }else{
            lua_pushstring(L, p.first.c_str());
        }
        lua_pushstring(L,p.second.c_str());
        lua_settable(L, -3);
    }
    for(std::pair<std::string, Table> p : t.m_tableMap){
        if(isNumber(p.first)){
            lua_pushnumber(L, stringToFloat(p.first));
        }else{
            lua_pushstring(L, p.first.c_str());
        }
        lua_pushtable(L,p.second);
        lua_settable(L, -3);
    }
    

    
}

int get_c_table(lua_State* L) {



    Table t,person;

    person.setString("name","albert");
    person.setFloat("age",23.5);

    t.setTable("person",person);
    t.setFloat("A",1);
    t.setFloat("B",2);

    t.setString("1","a");
    t.setString("2","B");

    lua_pushtable(L,t);
    

    // Return the table
    return 1;
}

Table lua_totable(lua_State* L,int index){
    Table t;
    return t;
}

int c_table_reader(lua_State* L) {
    return 0;
}

void run_test_lua(){
    lua_State *L = luaL_newstate(); // create a new Lua state
    luaL_openlibs(L); // load standard Lua libraries

    lua_pushcfunction(L, get_c_table);
    lua_setglobal(L, "get_c_table");

    lua_pushcfunction(L, c_table_reader);
    lua_setglobal(L, "c_table_reader");

    // load and execute the Lua script
    if (luaL_dofile(L, "test_lua.lua")) {
        printf("Error running Lua script: %s\n", lua_tostring(L, -1));
    }

    lua_close(L); // close the Lua state
}


int main() {
    //test_table();
    //test_is_number();
    run_test_lua();
    

}