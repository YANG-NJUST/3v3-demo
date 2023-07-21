#include "UGV.h"
#include <Windows.h>

#include "tinyxml2.h"

using namespace tinyxml2;


string UGV_NAME, UAV_NAME;
double UGV_Speed, UAV_Speed;
double UGV_DetectDis;

void sendMessage() {
    HWND hWnd = FindWindow(NULL, "ProjectChart");	//发送目标句柄 
    if (NULL != hWnd) {
        Struct param = "中文";  //用来发送的数据
        COPYDATASTRUCT data;           //使用COPYDATA的方式进行数据传递
        data.dwData = 100;               //指定传输的数据编码 
        data.cbData = param.length();  //指定数据大小
        data.lpData = &param;       //指向要传递到接收应用程序的数据的指针。可以为空
        SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&data);
    }
}


int main()
{
    tinyxml2::XMLDocument xmlDoc;

    // 加载XML文件
    XMLError result = xmlDoc.LoadFile("scenario.xml");
    if (result != XML_SUCCESS) {
        std::cout << "无法加载XML文件." << std::endl;
        return result;
    }

    // 获取根元素
    XMLElement* rootElement = xmlDoc.RootElement();
    if (rootElement == nullptr) {
        std::cout << "无法获取根元素." << std::endl;
        return XML_ERROR_FILE_READ_ERROR;
    }

    // 遍历子元素
    XMLElement* childElement = rootElement->FirstChildElement();
    while (childElement != nullptr) {
        const char* elementName = childElement->Name();
         
        // 处理不同的子元素
        if (strcmp(elementName, "UGV_AirDefence") == 0) {
            // 处理防空型无人战车
            UGV_NAME = childElement->FirstChildElement("name")->GetText();
            UGV_Speed = std::stod(childElement->FirstChildElement("maxSpeed")->GetText());
            UGV_DetectDis = std::stod(childElement->FirstChildElement("detectDis_Day")->GetText());
            double detectDis_Night = std::stod(childElement->FirstChildElement("detectDis_Night")->GetText());
            const char* weaponName = childElement->FirstChildElement("weapon")->GetText();
            double minFireRange = std::stod(childElement->FirstChildElement("weapon")->Attribute("min_fire_range"));
            double maxFireRange = std::stod(childElement->FirstChildElement("weapon")->Attribute("max_fire_range"));
            int weaponNum = std::stoi(childElement->FirstChildElement("weapon")->Attribute("num"));
            int maxKill = std::stoi(childElement->FirstChildElement("weapon")->Attribute("maxKill"));

            // 打印读取到的数据
            std::cout << "防空型无人战车" << std::endl;
            std::cout << "名称: " << UGV_NAME << std::endl;
            std::cout << "最大速度: " << UGV_Speed << " m/s" << std::endl;
            std::cout << "白天探测距离: " << UGV_DetectDis << " m" << std::endl;
            std::cout << "夜晚探测距离: " << detectDis_Night << " m" << std::endl;
            std::cout << "武器名称: " << weaponName << std::endl;
            std::cout << "最小射程: " << minFireRange << " m" << std::endl;
            std::cout << "最大射程: " << maxFireRange << " m" << std::endl;
            std::cout << "装配数量: " << weaponNum << std::endl;
            std::cout << "最大杀伤力: " << maxKill << std::endl;
            std::cout << std::endl;
        }
        else if (strcmp(elementName, "UAV") == 0) {
            // 处理无人侦察机
            UAV_NAME = childElement->FirstChildElement("name")->GetText();
            UAV_Speed = std::stod(childElement->FirstChildElement("cruiseSpeed")->GetText());
            double maxFlightHeight = std::stod(childElement->FirstChildElement("maxFlightHeight")->GetText());
            double reconnaissanceDis = std::stod(childElement->FirstChildElement("reconnaissanceDis")->GetText());
            const char* weaponName = childElement->FirstChildElement("weapon")->GetText();
            double minFireRange = std::stod(childElement->FirstChildElement("weapon")->Attribute("min_fire_range"));
            double maxFireRange = std::stod(childElement->FirstChildElement("weapon")->Attribute("max_fire_range"));
            int weaponNum = std::stoi(childElement->FirstChildElement("weapon")->Attribute("num"));
            int maxKill = std::stoi(childElement->FirstChildElement("weapon")->Attribute("maxKill"));

            // 打印读取到的数据
            std::cout << "无人侦察机" << std::endl;
            std::cout << "名称: " << UAV_NAME << std::endl;
            std::cout << "巡航速度: " << UAV_Speed << "m/s" << std::endl;
            std::cout << "最大飞行高度: " << maxFlightHeight << std::endl;
            std::cout << "侦察距离: " << reconnaissanceDis << " m" << std::endl;
            std::cout << "武器名称: " << weaponName << std::endl;
            std::cout << "最小射程: " << minFireRange << " m" << std::endl;
            std::cout << "最大射程: " << maxFireRange << " m" << std::endl;
            std::cout << "装配数量: " << weaponNum << std::endl;
            std::cout << "最大杀伤力: " << maxKill << std::endl;
            std::cout << std::endl;
        }
        else if (strcmp(elementName, "RedCamp") == 0) {
            // 处理红方阵营
            XMLElement* agentElement = childElement->FirstChildElement("agent");
            while (agentElement != nullptr) {
                const char* agentId = agentElement->Attribute("id");
                const char* agentName = agentElement->Attribute("name");
                double agentX = std::stod(agentElement->Attribute("x"));
                double agentY = std::stod(agentElement->Attribute("y"));
                double agentZ = std::stod(agentElement->Attribute("z"));

                // 打印读取到的数据
                std::cout << "红方阵营" << std::endl;
                std::cout << "代理ID: " << agentId << std::endl;
                std::cout << "代理名称: " << agentName << std::endl;
                std::cout << "坐标: (" << agentX << ", " << agentY << ", " << agentZ << ")" << std::endl;

                // 移动到下一个代理元素
                agentElement = agentElement->NextSiblingElement("agent");
            }
            std::cout << std::endl;
        }
        else if (strcmp(elementName, "BlueCamp") == 0) {
            // 处理蓝方阵营
            XMLElement* agentElement = childElement->FirstChildElement("agent");
            while (agentElement != nullptr) {
                const char* agentId = agentElement->Attribute("id");
                const char* agentName = agentElement->Attribute("name");
                double agentX = std::stod(agentElement->Attribute("x"));
                double agentY = std::stod(agentElement->Attribute("y"));
                double agentZ = std::stod(agentElement->Attribute("z"));

                // 打印读取到的数据
                std::cout << "蓝方阵营" << std::endl;
                std::cout << "代理ID: " << agentId << std::endl;
                std::cout << "代理名称: " << agentName << std::endl;
                std::cout << "坐标: (" << agentX << ", " << agentY << ", " << agentZ << ")" << std::endl;

                // 移动到下一个代理元素
                agentElement = agentElement->NextSiblingElement("agent");
            }
            std::cout << std::endl;
        }

        

        // 移动到下一个子元素
        childElement = childElement->NextSiblingElement();
    }




	vector<UGV*> red_agent, blue_agent;

	UGV* UGV_red_1 = new UGV("RED", "red_01", Position_ECEF(1, 1, 1), 1.0, 1.0, Weapon("weapon1", 0.0, 100.0, 10, 5));
	UGV* UGV_red_2 = new UGV("RED", "red_02", Position_ECEF(2, 2, 2), 2.0, 2.0, Weapon("weapon2", 0.0, 100.0, 10, 5));
	UGV* UGV_red_3 = new UGV("RED", "red_03", Position_ECEF(3, 3, 3), 3.0, 3.0, Weapon("weapon3", 0.0, 100.0, 10, 5));
	red_agent.emplace_back(UGV_red_1);
	red_agent.emplace_back(UGV_red_2);
	red_agent.emplace_back(UGV_red_3);

    UGV* UGV_blue_1 = new UGV("BLUE", "blue_01", Position_ECEF(1, 1, 1), 1.0, 1.0, Weapon("weapon1", 0.0, 100.0, 10, 5));
	UGV* UGV_blue_2 = new UGV("BLUE", "blue_02", Position_ECEF(2, 2, 2), 2.0, 2.0, Weapon("weapon2", 0.0, 100.0, 10, 5));
	UGV* UGV_blue_3 = new UGV("BLUE", "blue_03", Position_ECEF(3, 3, 3), 3.0, 3.0, Weapon("weapon3", 0.0, 100.0, 10, 5));
	blue_agent.emplace_back(UGV_blue_1);
	blue_agent.emplace_back(UGV_blue_2);
	blue_agent.emplace_back(UGV_blue_3);
	
	adevs::Digraph<string>* model = new adevs::Digraph<string>();

	model->add(UGV_red_1);
	model->add(UGV_red_2);
	model->add(UGV_red_3);

	model->add(UGV_blue_1);
	model->add(UGV_blue_2);
	model->add(UGV_blue_3);

	/*for (auto red : red_agent)
	{
		for (auto blue : blue_agent)
		{
			model->couple(red, red->out_enemy, blue, blue->in_enemy);
			model->couple(blue, blue->out_enemy, red, red->in_enemy);
		}
	}

	for (auto red : red_agent)
	{
		for (auto red_n : red_agent)
		{
			model->couple(red, red->out_friend, red_n, red_n->in_friend);
			model->couple(red_n, red_n->out_friend, red, red->in_friend);
		}
	}

	for (auto blue : blue_agent)
	{
		for (auto blue_n : blue_agent)
		{
			model->couple(blue, blue->out_friend, blue_n, blue_n->in_friend);
			model->couple(blue_n, blue_n->out_friend, blue, blue->in_friend);
		}
	}*/

	adevs::Simulator<IO_Type>* sim = new adevs::Simulator<IO_Type>(model);

	while (sim->nextEventTime() <= 1000)
	{
		cout << "=================" << sim->nextEventTime() << "=================" << endl;
		sim->execNextEvent();

        for (auto agent : model->models)
        {
            StateMsg* msg = dynamic_cast<UGV*>(agent)->getCurrentStateMsg();
        }

		Sleep(1000);
	}

	return 0;
}

