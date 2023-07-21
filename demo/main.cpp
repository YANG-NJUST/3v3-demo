#include "UGV.h"
#include <Windows.h>

#include "tinyxml2.h"

using namespace tinyxml2;


string UGV_NAME, UAV_NAME;
double UGV_Speed, UAV_Speed;
double UGV_DetectDis;

void sendMessage() {
    HWND hWnd = FindWindow(NULL, "ProjectChart");	//����Ŀ���� 
    if (NULL != hWnd) {
        Struct param = "����";  //�������͵�����
        COPYDATASTRUCT data;           //ʹ��COPYDATA�ķ�ʽ�������ݴ���
        data.dwData = 100;               //ָ����������ݱ��� 
        data.cbData = param.length();  //ָ�����ݴ�С
        data.lpData = &param;       //ָ��Ҫ���ݵ�����Ӧ�ó�������ݵ�ָ�롣����Ϊ��
        SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&data);
    }
}


int main()
{
    tinyxml2::XMLDocument xmlDoc;

    // ����XML�ļ�
    XMLError result = xmlDoc.LoadFile("scenario.xml");
    if (result != XML_SUCCESS) {
        std::cout << "�޷�����XML�ļ�." << std::endl;
        return result;
    }

    // ��ȡ��Ԫ��
    XMLElement* rootElement = xmlDoc.RootElement();
    if (rootElement == nullptr) {
        std::cout << "�޷���ȡ��Ԫ��." << std::endl;
        return XML_ERROR_FILE_READ_ERROR;
    }

    // ������Ԫ��
    XMLElement* childElement = rootElement->FirstChildElement();
    while (childElement != nullptr) {
        const char* elementName = childElement->Name();
         
        // ����ͬ����Ԫ��
        if (strcmp(elementName, "UGV_AirDefence") == 0) {
            // �������������ս��
            UGV_NAME = childElement->FirstChildElement("name")->GetText();
            UGV_Speed = std::stod(childElement->FirstChildElement("maxSpeed")->GetText());
            UGV_DetectDis = std::stod(childElement->FirstChildElement("detectDis_Day")->GetText());
            double detectDis_Night = std::stod(childElement->FirstChildElement("detectDis_Night")->GetText());
            const char* weaponName = childElement->FirstChildElement("weapon")->GetText();
            double minFireRange = std::stod(childElement->FirstChildElement("weapon")->Attribute("min_fire_range"));
            double maxFireRange = std::stod(childElement->FirstChildElement("weapon")->Attribute("max_fire_range"));
            int weaponNum = std::stoi(childElement->FirstChildElement("weapon")->Attribute("num"));
            int maxKill = std::stoi(childElement->FirstChildElement("weapon")->Attribute("maxKill"));

            // ��ӡ��ȡ��������
            std::cout << "����������ս��" << std::endl;
            std::cout << "����: " << UGV_NAME << std::endl;
            std::cout << "����ٶ�: " << UGV_Speed << " m/s" << std::endl;
            std::cout << "����̽�����: " << UGV_DetectDis << " m" << std::endl;
            std::cout << "ҹ��̽�����: " << detectDis_Night << " m" << std::endl;
            std::cout << "��������: " << weaponName << std::endl;
            std::cout << "��С���: " << minFireRange << " m" << std::endl;
            std::cout << "������: " << maxFireRange << " m" << std::endl;
            std::cout << "װ������: " << weaponNum << std::endl;
            std::cout << "���ɱ����: " << maxKill << std::endl;
            std::cout << std::endl;
        }
        else if (strcmp(elementName, "UAV") == 0) {
            // ������������
            UAV_NAME = childElement->FirstChildElement("name")->GetText();
            UAV_Speed = std::stod(childElement->FirstChildElement("cruiseSpeed")->GetText());
            double maxFlightHeight = std::stod(childElement->FirstChildElement("maxFlightHeight")->GetText());
            double reconnaissanceDis = std::stod(childElement->FirstChildElement("reconnaissanceDis")->GetText());
            const char* weaponName = childElement->FirstChildElement("weapon")->GetText();
            double minFireRange = std::stod(childElement->FirstChildElement("weapon")->Attribute("min_fire_range"));
            double maxFireRange = std::stod(childElement->FirstChildElement("weapon")->Attribute("max_fire_range"));
            int weaponNum = std::stoi(childElement->FirstChildElement("weapon")->Attribute("num"));
            int maxKill = std::stoi(childElement->FirstChildElement("weapon")->Attribute("maxKill"));

            // ��ӡ��ȡ��������
            std::cout << "��������" << std::endl;
            std::cout << "����: " << UAV_NAME << std::endl;
            std::cout << "Ѳ���ٶ�: " << UAV_Speed << "m/s" << std::endl;
            std::cout << "�����и߶�: " << maxFlightHeight << std::endl;
            std::cout << "������: " << reconnaissanceDis << " m" << std::endl;
            std::cout << "��������: " << weaponName << std::endl;
            std::cout << "��С���: " << minFireRange << " m" << std::endl;
            std::cout << "������: " << maxFireRange << " m" << std::endl;
            std::cout << "װ������: " << weaponNum << std::endl;
            std::cout << "���ɱ����: " << maxKill << std::endl;
            std::cout << std::endl;
        }
        else if (strcmp(elementName, "RedCamp") == 0) {
            // ����췽��Ӫ
            XMLElement* agentElement = childElement->FirstChildElement("agent");
            while (agentElement != nullptr) {
                const char* agentId = agentElement->Attribute("id");
                const char* agentName = agentElement->Attribute("name");
                double agentX = std::stod(agentElement->Attribute("x"));
                double agentY = std::stod(agentElement->Attribute("y"));
                double agentZ = std::stod(agentElement->Attribute("z"));

                // ��ӡ��ȡ��������
                std::cout << "�췽��Ӫ" << std::endl;
                std::cout << "����ID: " << agentId << std::endl;
                std::cout << "��������: " << agentName << std::endl;
                std::cout << "����: (" << agentX << ", " << agentY << ", " << agentZ << ")" << std::endl;

                // �ƶ�����һ������Ԫ��
                agentElement = agentElement->NextSiblingElement("agent");
            }
            std::cout << std::endl;
        }
        else if (strcmp(elementName, "BlueCamp") == 0) {
            // ����������Ӫ
            XMLElement* agentElement = childElement->FirstChildElement("agent");
            while (agentElement != nullptr) {
                const char* agentId = agentElement->Attribute("id");
                const char* agentName = agentElement->Attribute("name");
                double agentX = std::stod(agentElement->Attribute("x"));
                double agentY = std::stod(agentElement->Attribute("y"));
                double agentZ = std::stod(agentElement->Attribute("z"));

                // ��ӡ��ȡ��������
                std::cout << "������Ӫ" << std::endl;
                std::cout << "����ID: " << agentId << std::endl;
                std::cout << "��������: " << agentName << std::endl;
                std::cout << "����: (" << agentX << ", " << agentY << ", " << agentZ << ")" << std::endl;

                // �ƶ�����һ������Ԫ��
                agentElement = agentElement->NextSiblingElement("agent");
            }
            std::cout << std::endl;
        }

        

        // �ƶ�����һ����Ԫ��
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

