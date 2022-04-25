#pragma once

// C++
#include <cstdint>

// ES32
#include <driver\gpio.h>

//menu
#define FW_UPDATE_STATE 2
#define MENU_HEIGHT 25
#define MENU_LENGTH 7

//BLE OTA
#define BLE_DEVICE_NAME "Watchy BLE OTA"
#define WATCHFACE_NAME "Watchy 7 Segment"
#define SOFTWARE_VERSION_MAJOR 1
#define SOFTWARE_VERSION_MINOR 0
#define SOFTWARE_VERSION_PATCH 0
#define HARDWARE_VERSION_MAJOR 1
#define HARDWARE_VERSION_MINOR 0
//Versioning
#define WATCHY_LIB_VER "1.4.0"

namespace watchy_config
{

namespace display
{
// Display
constexpr std::int16_t kDisplayWidth{200};
constexpr std::int16_t kDisplayHeight{200};

// display pins
constexpr std::uint8_t kCS{5}; // Unsure what this means.
constexpr std::uint8_t kDC{10}; // Unsure what this means.
constexpr std::uint8_t kReset{9};
constexpr std::uint8_t kBusy{19};
};

namespace pins
{

// RTC pins
constexpr gpio_num_t rtc_pin{GPIO_NUM_27};

#define ARDUINO_WATCHY_V20 1

#if defined (ARDUINO_WATCHY_V10)
	constexpr std::uint64_t up_btn_mask{GPIO_SEL_32};
#elif defined (ARDUINO_WATCHY_V15)
	constexpr std::uint64_t up_btn_mask{GPIO_SEL_32};
#elif defined (ARDUINO_WATCHY_V20)
	constexpr std::uint64_t up_btn_mask{GPIO_SEL_35};
	constexpr std:: uint8_t batt_adc_pin{34};
#endif

// btn pins & masks
constexpr std::uint64_t menu_btn_mask{GPIO_SEL_26};
constexpr std::uint64_t back_btn_mask{GPIO_SEL_25};
constexpr std::uint64_t down_btn_mask{GPIO_SEL_4};
constexpr std::uint64_t btn_pin_mask{menu_btn_mask|back_btn_mask|up_btn_mask|down_btn_mask};

};
};

namespace wc  = watchy_config;
namespace wcd = watchy_config::display;
namespace wcp = watchy_config::pins;
