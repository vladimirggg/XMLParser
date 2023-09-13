#include "Parser.h"

std::vector<item> XMLParser::tokenise(std::string info)
{
    std::vector<item> res;
    for (size_t i = 0; i < info.length()-1; )
    {
        // letter from the scheme [N,A,D] and the corresponding string
        item element;
        if (info[i] == '<')
        {
            // that would be the name of the element: <person>
            //                                        ^------^
            i++;
            element.initial = 'N';
            while (info[i] != '>' && info[i] != ' ')
            {
                element.val += info[i++];
            }
        }
        else if (info[i] == ' ')
        {
            // that would be the attribute of the element: <person id = "0">
            //                                                    ^--------^
            i++;
            element.initial = 'A';
            while (info[i] != '>')
            {
                element.val += info[i++];
            }
        }
        else
        {
            // that would be the data of the element: <name>John Smith</name>
            //                                             ^----------^
            i++;
            if(info[i] == '\n') continue;
            element.initial = 'D';
            while (info[i] != '<')
            {
                element.val += info[i++];
            }
        }

        if(info[i] == '>' && info[i + 1] == '<') i++;
        // in the case of 2 tags one after another: <person><name>
        res.push_back(element);
    }
    return res;
}

void XMLParser::createUsing(std::string str)
{
    Node* curr = this->root;

    std::vector<item> info = tokenise(str);
    for (item el : info)
    {
        if (el.initial == 'N')
        {
            if (el.val[0] == '/') curr = curr->parent;

            else {
                Node *newChild = new Node(el.val, "", "", std::vector<Node *>(), nullptr);
                curr->children.push_back(newChild);
                Node *temp = curr;
                curr = curr->children[curr->children.size() - 1];
                curr->parent = temp;
            }
        }
        else if (el.initial == 'A') curr->attribute = el.val;

        else if (el.initial == 'D') curr->data = el.val;

    }
    this->root = curr;
}

std::vector<std::string> XMLParser::splitIntoElements(std::string query)
{
    std::vector<std::string> elements;
    std::string temp = "";
    for (char ch : query){
        // skip the second bracket
        if(ch == ']') continue;
        // if it is a letter, keep building the word
        if ((tolower(ch) >= 'a' && tolower(ch) <= 'z') || ch == ' ') temp += ch;
        else{
            // push the word
            if(temp != ""){
                elements.push_back(temp);
                temp = "";
            }

            // push the symbol
            temp += ch;
            elements.push_back(temp);
            temp = "";
        } 
    }
    if(temp != "")
        elements.push_back(temp);
    return elements;
}

void XMLParser::queryExec(std::string query)
{
    std::regex slashPattern(R"(\w+\/\w+$)");
    std::regex slashIndexPattern(R"(\w+\/\w+\[\d+\])");
    std::regex atPattern(R"(\w+\[@(\w+)\])");
    std::regex dataMatchPattern(R"(\w+\[\w+\=(\w|\ )+\]\/\w+)");

    std::vector<std::string> elements = splitIntoElements(query);

    if(std::regex_match(query,slashPattern)){
        // elements vector looks like: {"parent", "/", "child"}
        std::vector<std::string> result;
        slash(elements, result, root);
        for (std::string s : result)
        {
            std::cout<<s<<std::endl;
        }
    }
    
    else if(std::regex_match(query,slashIndexPattern)){
        // elements vector looks like: {"parent", "/", "child", "[", "index"}
        
        std::vector<std::string> result;
        slash(elements, result, root);
        //get the index from the query
        try
        {
            int index = std::stoi(elements[4]);
            std::cout<<result[index]<<'\n';
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    
    else if(std::regex_match(query,atPattern)){
        // elements vector looks like: {"parent", "[", "@", "attribute"}
        std::vector<std::string> result;
        atOperator(elements, result, root);

        for (std::string s : result)
        {
            std::cout<<s<<std::endl;
        }
    }
    
    else if(std::regex_match(query,dataMatchPattern)){
        // elements looks like {"parent", "[", "filterChild", "=", "filter", "/", "endChild"}
        std::vector<std::string> result;
        dataMatch(elements,result,root);

        for (std::string s : result)
        {
            std::cout<<s<<'\n';
        }
    }
    else{
        std::cout<<"Unknown query!\n";
    }
}

void XMLParser::slash(const std::vector<std::string>& elements,std::vector<std::string>& result, Node* curr)
{
    // elements vector looks like: {"parent", "/", "child"}
    if(curr->elementName == elements[2] && curr->parent->elementName == elements[0])
        result.push_back(curr->data);

    if(curr->children.empty()) return;

    for (Node* el : curr->children)
        slash(elements, result, el);
}

void XMLParser::atOperator(const std::vector<std::string>& elements, std::vector<std::string>& result, Node* curr)
{
    // elements vector looks like: {"parent", "[", "@", "attribute"}

    if(curr->elementName == elements[0] && curr->attribute.find(elements[3]) != std::string::npos){
        for (int i = 0; i < curr->attribute.length(); i++)
        {
            if (curr->attribute[i] == '=')
            {
                // in order to ignore the = we use i + 1
                result.push_back(curr->attribute.substr(i+1,std::string::npos));
                break;
            }
        }
    }

    if(curr->children.empty()) return;

    for (Node* el : curr->children)
    {
        atOperator(elements, result, el);
    }
}

void XMLParser::dataMatch(const std::vector<std::string>& elements, std::vector<std::string>& result, Node* curr)
{
    // elements looks like {"parent", "[", "filterChild", "=", "filter", "/", "endChild"}

    if(curr->elementName == elements[2] && curr->data == elements[4] && curr->parent->elementName == elements[0]){
        for (Node* child : curr->parent->children)
        {
            if(child->elementName == elements[6]){
                result.push_back(child->data);
            }
        }
        
    }
    if (curr->children.empty()) return;
    
    for (Node* el : curr->children)
        dataMatch(elements, result, el);
}

void XMLParser::menu()
{
    std::cout<<"\nPossible operators ('/', '[]', '@', '=')\n"
            <<"- A/B returns the data of all the B children of A\n"
            <<"- A/B[0] returns the first of the A/B\n"
            <<"- A[@attribute] returns returns the attribute(s) of all A elements\n"
            <<"- A[B = data] returns all B children of A whose data matches data\n"
            <<" Type 'exit' to quit\n";
    while(true){
        std::cout<<"Enter your query: ";
        std::string query;
        std::getline(std::cin,query);
        if(query == "exit") return;
        queryExec(query);
    }
    
}