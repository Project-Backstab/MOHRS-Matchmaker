#include <service/discord.h>
#include <globals.h>
#include <settings.h>
#include <logger.h>

Service::Discord::Discord() : _bot("")
{
	std::shared_lock<std::shared_mutex> guard(g_settings_mutex); // settings lock (read)

	this->_bot.token = g_settings["discord"]["token"].asString();

	this->_bot.on_ready([discord = this](const dpp::ready_t& event)
	{
		discord->_guilds = event.guilds;
		
		discord->Send("MOHRS-Matchmaker server started!");
	});
}

void Service::Discord::Start()
{
	this->_bot.start(dpp::st_wait);
}

void Service::Discord::Send(const std::string& msg)
{
	for(dpp::snowflake guild_id : this->_guilds)
	{
		this->_bot.channels_get(guild_id, [discord = this, msg](const dpp::confirmation_callback_t& callback)
		{
			if (callback.is_error())
			{
				return;
			}
			
			std::shared_lock<std::shared_mutex> guard(g_settings_mutex); // settings lock (read)
			
			dpp::channel_map channels = callback.get<dpp::channel_map>();
			
			for (const auto& pair : channels)
			{			
				if(pair.second.name == g_settings["discord"]["channel"].asString())
				{
					discord->_bot.message_create(dpp::message(pair.first, msg));

					Logger::info("<-- " + msg, Service::Type::Discord);
				}
			}
		});
	}
}

