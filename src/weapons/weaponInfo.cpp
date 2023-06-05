#include "weaponInfo.h"
#include <sp2/io/keyValueTreeLoader.h>
#include <sp2/random.h>
#include <sp2/stringutil/convert.h>
#include <unordered_map>

std::unordered_map<sp::string, WeaponInfo> weapons;

void WeaponInfo::init()
{
    sp::KeyValueTreePtr tree = sp::io::KeyValueTreeLoader::loadResource("weapons/definitions.txt");
    for(auto data : tree->getFlattenNodesByIds())
    {
        WeaponInfo info;
        info.name = data.first;
        info.auto_fire = data.second["auto_fire"] == "true";
        
        info.ammo_per_clip = sp::stringutil::convert::toInt(data.second["ammo_per_clip"]);
        info.cooldown_delay = sp::stringutil::convert::toFloat(data.second["cooldown_delay"]);
        info.reload_delay = sp::stringutil::convert::toFloat(data.second["reload_delay"]);

        info.projectile_count = sp::stringutil::convert::toInt(data.second["projectile_count"]);
        info.projectile_spread = sp::stringutil::convert::toFloat(data.second["projectile_spread"]);
        info.trace_shot = sp::stringutil::convert::toBool(data.second["trace_shot"]);
        info.pass_trough_enemies = sp::stringutil::convert::toBool(data.second["pass_trough_enemies"]);
        
        info.damage_per_hit = sp::stringutil::convert::toFloat(data.second["damage_per_hit"]);
        info.range = sp::stringutil::convert::toFloat(data.second["range"]);
    
        info.auto_aim = data.second["auto_aim"] == "true";
        
        float average_time_per_shot = (info.reload_delay + info.cooldown_delay * float(info.ammo_per_clip - 1)) / float(info.ammo_per_clip);
        float damage_per_shot = float(info.projectile_count) * info.damage_per_hit;
        float raw_dps = damage_per_shot / average_time_per_shot;
        if (!info.auto_fire)
            average_time_per_shot += 0.07; //when not using auto fire, the user still needs to press the button which adds some delay between shots.
        if (info.pass_trough_enemies)
            damage_per_shot *= 1.5;
        float guessed_dps = damage_per_shot / average_time_per_shot;
        float burst_dps = damage_per_shot / info.cooldown_delay;
        LOG(Debug, "Weapon:", info.name, "Raw DPS:", raw_dps, "Guessed DPS:", guessed_dps, "Burst DPS:", burst_dps);
        
        weapons[info.name] = info;
    }
}

WeaponInfo& WeaponInfo::get(sp::string name)
{
    return weapons[name];
}

WeaponInfo& WeaponInfo::random()
{
    auto it = weapons.begin();
    std::advance(it, sp::irandom(0, weapons.size() - 1));
    return it->second;
}
