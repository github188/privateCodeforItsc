#include <string>
#include <cstring>
#include "communication.h"
#include "its.h"
#include "light.h"
#include "cycle.h"
#include "log.h"
#include "ctrl.h"
#include "singleton.h"

Its::~Its()
{
	if (cycle != nullptr)
		delete cycle;
	if (nextCycle != nullptr)
		delete nextCycle;
}

void Its::UpdateRule()
{
	if (!ruleUpdate)
		return;
	ControlRule rule = newRule;
	ControlRule cur = curRule;
	ruleUpdate = false;

	rule.Print(__func__);
	if (rule.SpecialMode())
	{	//黄闪、全红或关灯则立即改变控制规则
		rule.ctrlId = 0;
		curRule = rule;
		if (cycle != nullptr)
			cycle->SpecialCtrl(rule.ctrlMode);
	}
	else if (rule.ctrlMode == STEP_MODE)
	{
		if (cycle == nullptr || cycle->StepInvalid(rule.ctrlId))
		{	//说明步进失败，需要恢复当前周期的使用规则
			Singleton<Ctrl>::GetInstance().RecoverRule(cur);
			return;
		}
		if (cur.ctrlMode == STEP_MODE && rule.ctrlId == 0)
			cycle->Step(cycle->NextStage());	//如果之前就处于步进模式，再次单步步进时直接步进到下个阶段
		else
			cycle->Step(rule.ctrlId);
		curRule = rule;
	}
	else
	{
		if (cur.SpecialMode() && cycle != nullptr)
		{	//说明之前是处于特殊控制中
			delete cycle;
			cycle = nullptr;
			return;
		}
		if (cycle != nullptr && cur.ctrlMode == STEP_MODE)
		{
			cycle->CancelStep();
			curRule = cycle->rule;
		}
	}
}

void Its::RealtimeStat()
{
	Communication &communication = Singleton<Communication>::GetInstance();
	char buf[256] = {0};
	std::string data;
	ControlRule cur = curRule;

	std::sprintf(buf, "{\"ctrlType\":%d,\"ctrlMode\":%d,\"ctrlId\":%d,",
		cur.ctrlType, cur.ctrlMode, cur.ctrlId);
	data += buf;
	if (cycle == nullptr)
	{
		data.back() = '}';
		communication.SendRealtimeStat(data);
		return;
	}

	if (!cycle->desc.empty())
	{	
		data += R"("desc":")";
		data += cycle->desc + R"(",)";
	}
	data += R"("cycle":)";
	data += std::to_string((int)cycle->cycleTime);
	data += R"(, "start":")";
	data += Log::ConvertTime(cycle->beginTime);
	data += R"(", "time":")";
	data += Log::CurrentTime();
	data += R"(","current":)";
	data += std::to_string((int)(cycle->cycleTime - cycle->leftTime + 1));	//+1是因为平台要求当前运行时间从1开始
	if (!cycle->phaseTable.empty())
	{
		data += R"(,"phase":[)";
		for (auto &it : cycle->phaseTable)
		{
			Phase &phase = it.second;
			std::memset(buf, 0, sizeof(buf));
			ColorStep cs = phase.MotorColorStep();
			std::sprintf(buf, "{\"id\":%d,\"desc\":\"%s\",\"status\":%d,\"countdown\":%d,\"total\":%d,\"now\":%d},", 
				phase.phaseId, phase.desc.c_str(), cs.status, cs.countdown, phase.Total(), phase.Used());
			data += buf;
		}
		data.back() = ']';
	}
	if (!cycle->ringTable.empty())
	{
		data += R"(,"ring":[)";
		for (auto &ring : cycle->ringTable)
		{
			data +='[';
			for(auto &phaseid : ring.turn)
			{
				std::memset(buf, 0, sizeof(buf));
				std::sprintf(buf, "%d,", phaseid);
				data += buf;
			}
			data.back()=']';
			data += ',';
		}
		data.back() = ']';
	}
	data += R"(,"channel":[)";
	for (auto &channel : cycle->channelTable)
	{
		if (channel.type != CHANNEL_TYPE_UNUSED)
		{
			std::memset(buf, 0, sizeof(buf));
			std::sprintf(buf, "{\"id\":%d,\"status\":%d,\"countdown\":%d},",
					channel.id, channel.status, channel.countdown);
			data += buf;
		}
	}
	data.back() = ']';
	data += '}';
	communication.SendRealtimeStat(data);
}

void Its::Run()
{
	auto GetNextCycle = [this]{
		rwlock.r_lock();
		if (nextCycle != nullptr)
			cycle = nextCycle->Clone();
		rwlock.r_unlock();
		if (cycle != nullptr)
			curRule = cycle->rule;
	};
	Light light;
	light.start();

	while (true)
	{
		UpdateRule();
		
		if (cycle != nullptr)
		{
			cycle->Excute();
			light.LightChannel(cycle->channelTable);
			RealtimeStat();
			if (cycle->Over())
			{
				delete cycle;
				GetNextCycle();
			}
		}
		else
		{
			light.LightChannel(curRule.load().ctrlMode);
			RealtimeStat();
			GetNextCycle();
		}
	}
}

void Its::UpdateCycle(Cycle *next)
{
	if (next == nullptr)
		return;
	rwlock.w_lock();
	if (nextCycle != nullptr)
		delete nextCycle;
	nextCycle = next;
	rwlock.w_unlock();
}