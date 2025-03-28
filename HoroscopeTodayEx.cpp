#include "HoroscopeToday.h"

#include <sstream>
#include <string>
#include <ctime>
#include <map>
#include <random>

struct UserData
{
    long long id = -1 ;
    long long lasterDate = -1;
    int scope;
};

std::map<int,UserData> UserDatas;

Plugin_Extern const char * PluginName()
{
    return "今日运势Ex";
}

Plugin_Extern void Init()
{
    // 作为触发器时parm为MessageType，此处为at
    // parm is MessageType when used as a trigger, value is "at"
    SubmitSpecialFunction(Message_Tirgger,"TriggerAt",at);
}

Plugin_Extern bool TriggerAt(MessageContent message)
{
    std::istringstream str(message.content);
    std::string parm;
    str>>parm;// 空格切分字符串 | Split string

    // 判断第一个值
    // Determine the first value
    if (parm == "今日运势ex" || parm == "今日运势Ex" || parm == "今日运势EX")
        HoroscopeToday(message);

    return true;
}

void HoroscopeToday(MessageContent message)
{
    time_t nowtime;
    time(&nowtime);
    tm * p = localtime(&nowtime);

    long long date = (p->tm_year+1900)*10000 + (p->tm_mon+1)*100 + p->tm_mday;// 获取日期 | Get the date

    UserData ud;

    if (UserDatas.find(message.sendID.userID) != UserDatas.end())
    {
        ud = UserDatas[message.sendID.userID];
    }
    else
    {
        ud.id = message.sendID.userID;
    }

    std::stringstream ss;

    ss << "\n";

    if (ud.lasterDate >= date)//今日有
    {
        ss<<"缓存中显示今天已经测过了！\n";//The cache shows that it has been tested today!
    }
    else
    {
        ud.scope = RandARumber100();
        ud.lasterDate = date;
    }

    ss<<"今日指数EX："<<ud.scope;

    message.content = ss.str().c_str();
/*
    message为传入的消息，其中有user的ID、群聊还是私聊、群联ID(如果是群聊)
    这里作为触发器,只有at时调用，因此消息类型为at（只在群聊才能触发，可能会改）
    无需再将at设为消息类型了，因为就是

    message is the incoming message, which contains the user ID, group or private chat, and group ID(if group chat).
    This is used as a trigger, which is only called at, so the message type is at (only triggered in group chat, may change).
    You no longer need to set at as the message type, because it is
*/

    UserDatas[message.sendID.userID] = ud;

    BotSendMessage(message);

}

int RandARumber100()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<double> dist(0.3);

    double r = dist(gen) *10 / 17;

    if (r >= 10) r = 10;
    if (r <= 0) r = 0;

    return int(round(r*10));
}