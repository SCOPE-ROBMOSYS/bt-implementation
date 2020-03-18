#ifndef SKILLCONFIGREADER_H
#define SKILLCONFIGREADER_H

#include <string>
#include <vector>

using namespace std;

struct SkillConfig {
    bool specify_port_name_attribute;
    vector<string> port_name_list;
};

class SkillConfigReader
{
public:
    SkillConfigReader(string file);

    SkillConfig ReturnConfig();
    bool ReadConfig();
    bool init();

    string file_;
    SkillConfig SC_; // result
private:
};

#endif // SKILLCONFIGREADER_H
