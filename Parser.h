#include <vector>
#include <iostream>
#include <string>
#include <regex>
#pragma once

struct Node{
    std::string elementName;
    std::string attribute;
    std::string data;
    std::vector<Node*> children;
    Node* parent;

    Node(std::string elementName, std::string attribute, std::string data, std::vector<Node*> children, Node* parent) :
            elementName(elementName), attribute(attribute), data(data), children(children), parent(parent) {}
};

struct item{
    char initial;
    std::string val;
    // action cheatsheet : N - elementName; A - attribute; D - data;
    // <initial,val>
    // <person> -> item = {'N',"person"}
};

class XMLParser
{
    Node* root;

    void queryExec(std::string query);

    void slash(const std::vector<std::string>& elements,std::vector<std::string>& result, Node* curr);

    void atOperator(const std::vector<std::string>& elements,std::vector<std::string>& result, Node* curr);

    void dataMatch(const std::vector<std::string>& elements,std::vector<std::string>& result, Node* curr);

    std::vector<item> tokenise(std::string info);

    std::vector<std::string> splitIntoElements(std::string query);

public:
    XMLParser() : root(new Node("root", "", "", std::vector<Node*>(), nullptr)){};

    void createUsing(std::string str);

    void menu(); 
};