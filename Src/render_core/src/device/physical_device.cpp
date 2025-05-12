
#include "render_core/device/physical_device.h"

PhysicalDevice::PhysicalDevice()
{
}

PhysicalDevice::~PhysicalDevice()
{
}

void PhysicalDevice::CreatePhysicalDevice()
{
    NY_ASSERT( mInstance )

    auto instance = mInstance->GetRaw();
    vk::PhysicalDevice physicalDevice = instance.enumeratePhysicalDevices().front();
    mPhysicalDevice = vk::SharedPhysicalDevice( physicalDevice, mInstance->GetShared() );

    // get the QueueFamilyProperties of the first PhysicalDevice
    // std::vector<vk::QueueFamilyProperties> queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

    // get the first index into queueFamilyProperties which supports graphics
    // auto propertyIterator = std::find_if( queueFamilyProperties.begin(),
    //                                       queueFamilyProperties.end(),
    //                                       []( vk::QueueFamilyProperties const &qfp )
    //                                       { return qfp.queueFlags & vk::QueueFlagBits::eGraphics; } );
    // i64 graphicsQueueFamilyIndex = std::distance( queueFamilyProperties.begin(), propertyIterator );
    // NY_ASSERT( static_cast<usize>( graphicsQueueFamilyIndex ) < queueFamilyProperties.size() )

    // // create a Device
    // float queuePriority = 0.0f;
    // vk::DeviceQueueCreateInfo deviceQueueCreateInfo( vk::DeviceQueueCreateFlags(),
    //                                                  static_cast<u32>( graphicsQueueFamilyIndex ),
    //                                                  1,
    //                                                  &queuePriority );
    // vk::Device device =
    //     physicalDevice.createDevice( vk::DeviceCreateInfo( vk::DeviceCreateFlags(), deviceQueueCreateInfo ) );
    // vk::SharedDevice sharedDevice( device );
}

void PhysicalDevice::DestroyPhysicalDevice()
{
    mPhysicalDevice.reset();
}
