#pragma once

#include "hw/hardware.h"
#include "core/engine.h"
#include "nocopy.h"

namespace spotykach {

class Settings {
public:
    static constexpr uint8_t kDataVersion = 5;
    Settings() = default;
    ~Settings() = default;

    struct Data {
        Data() { memset(&offset, 0, sizeof(float) * Hardware::CV_LAST); }
        ~Data() = default;

        float offset[Hardware::CV_LAST];
        infrasonic::CalibratedVOct::ReferenceReadings v_oct_a;
        infrasonic::CalibratedVOct::ReferenceReadings v_oct_b;
        uint8_t version = 1;

        bool operator != (const Data& other) const { return true; }
    };

    struct EngineData {
        EngineData() = default;
        ~EngineData() = default;

        uint8_t type = static_cast<uint8_t>(EngineType::Tape);

        bool operator != (const EngineData& other) const
        {
            return type != other.type;
        }
    };

    void init(Hardware& hw) {
        _storage.Init(hw.seed.qspi, _data, storage_version);
        _engine_storage.Init(hw.seed.qspi, _engine_data, engine_storage_offset);
    }

    inline bool is_user_defined() 
    {
        return static_cast<int>(_storage.GetState()) == 2; //USER
    }

    inline Data& data() { 
        return _data; 
    }

    void read() 
    {
        _data = _storage.GetSettings();
        _engine_data = _engine_storage.GetSettings();
    }

    bool write() 
    {
        _storage.Save(_data);
        read();
        return is_user_defined();
    }

    bool write_engine()
    {
        _engine_storage.Save(_engine_data);
        _engine_data = _engine_storage.GetSettings();
        return static_cast<int>(_engine_storage.GetState()) == 2;
    }

    inline EngineData& engine_data()
    {
        return _engine_data;
    }


private:
    NOCOPY(Settings)

    static constexpr char slug[4] = "cal";
    static constexpr char engine_slug[4] = "eng";
    static constexpr uint8_t storage_version = 1;
    static constexpr uint8_t engine_storage_version = 3;
    static constexpr uint32_t engine_storage_offset = 0x100;
    daisy::PersistentStorage<Data, slug, storage_version> _storage;
    daisy::PersistentStorage<EngineData, engine_slug, engine_storage_version> _engine_storage;
    Data _data;
    EngineData _engine_data;
};

};
