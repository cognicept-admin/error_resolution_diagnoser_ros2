#include <gtest/gtest.h>
#include <fstream>
#include <rosrect-listener-agent-ros2/robot_event.h>

using namespace web::json;                  // JSON features
using namespace web;                        // Common features like URIs.

// Create test object
RobotEvent event_instance;

// Create sample log  
std::vector<std::vector<std::string>> sample_log;
std::string level = "Error";
std::string cflag = "Null";
std::string module = "Null";
std::string source = "bt_navigator";
std::string message = "Navigation failed";
std::string description = "Null";
std::string resolution = "Null";

// Hold the record
std::vector<std::string> event_details;

TEST(RobotEventTestSuite, getLogTest)
{
  // Sample message
  rcl_interfaces::msg::Log data;
  data.level = data.ERROR;
  data.name = source;
  data.msg = message;  
  rcl_interfaces::msg::Log::SharedPtr rosmsg(new rcl_interfaces::msg::Log(data));

  // For simple get test, no need for ECS 
  json::value msgInfo = json::value::null();

  // Declare log variable
  std::vector<std::vector<std::string>> updatedLog;
  std::vector<std::string> currentRow;

  // Update log
  event_instance.update_log(rosmsg, msgInfo);

  // Check number of elements in event log
  // Get log
  updatedLog = event_instance.get_log();
  
  // Check if there is only one row
  ASSERT_EQ(updatedLog.size(), 1);
  
  // Check if there are 9 elements in that one row
  int expElements = 9;
  currentRow = updatedLog[0];
  ASSERT_EQ(currentRow.size(), expElements);

  // Clear event
  event_instance.clear();
}

TEST(RobotEventTestSuite, updateLogROSTest)
{
  // Sample message
  rcl_interfaces::msg::Log data;
  data.level = data.ERROR;
  data.name = source;
  data.msg = message;  
  rcl_interfaces::msg::Log::SharedPtr rosmsg(new rcl_interfaces::msg::Log(data));

  // For ROS test, no need for ECS 
  json::value msgInfo = json::value::null();

  // Declare log variable
  std::vector<std::vector<std::string>> updatedLog;
  std::vector<std::string> currentRow;

  // Update log
  event_instance.update_log(rosmsg, msgInfo);

  // Expected log
  // For ROS, cflag is Null
  cflag = "Null";
  event_details.push_back(level);
  event_details.push_back(cflag);
  event_details.push_back(module);
  event_details.push_back(source);
  event_details.push_back(message);
  event_details.push_back(description);
  event_details.push_back(resolution);
  
  // Get log
  updatedLog = event_instance.get_log();
  currentRow = updatedLog[0];
  
  // Check if content is equal
  for(int idx = 1; idx < currentRow.size()-1; idx++){
    ASSERT_EQ(event_details[idx-1], currentRow[idx]);    
  }

  // Clear event
  event_instance.clear();
  // Clear expected log
  event_details.clear();
}

// TEST(RobotEventTestSuite, updateLogDBTest)
// {
//   // Sample message
//   rcl_interfaces::msg::Log data;
//   data.level = data.ERROR;
//   data.name = source;
//   data.msg = message;  
//   rcl_interfaces::msg::Log::SharedPtr rosmsg(new rcl_interfaces::msg::Log(data));

//   // For DB test, need for ECS, manually construct an ECS response so we don't rely on ECS connection 
//   json::value msgInfo = json::value::object();
//   // For DB, cflag is NOT Null
//   cflag = "true";
//   event_details.push_back(level);
//   event_details.push_back(cflag);
//   event_details.push_back("Navigation");
//   event_details.push_back(source);
//   event_details.push_back(message);
//   event_details.push_back("The robot is unable to move around. This usually means the robot is mislocalized or there is an obstacle.");
//   event_details.push_back("Relocalize the robot using intervention, assign a sample goal. If that does not work, use teleoperation to nudge the robot from the impossible position. If that does not work, escalate to property.");

//   // Declare log variable
//   std::vector<std::vector<std::string>> updatedLog;
//   std::vector<std::string> currentRow;

//   // Create keys
//   utility::string_t codeKey(U("error_code"));
//   utility::string_t lvlKey(U("error_level"));
//   utility::string_t cfKey(U("compounding_flag"));
//   utility::string_t modKey(U("error_module"));
//   utility::string_t srcKey(U("error_source"));
//   utility::string_t txtKey(U("error_text"));
//   utility::string_t descKey(U("error_description"));
//   utility::string_t resKey(U("error_resolution"));

//   // Assign key-value
//   msgInfo[codeKey] = json::value::string("Null");
//   msgInfo[lvlKey] = json::value::string(level);
//   msgInfo[cfKey] = json::value::boolean(true);
//   msgInfo[modKey] = json::value::string("Navigation");
//   msgInfo[srcKey] = json::value::string("/move_base");
//   msgInfo[txtKey] = json::value::string(message);
//   msgInfo[descKey] = json::value::string("The robot is unable to move around. This usually means the robot is mislocalized or there is an obstacle.");
//   msgInfo[resKey] = json::value::string("Relocalize the robot using intervention, assign a sample goal. If that does not work, use teleoperation to nudge the robot from the impossible position. If that does not work, escalate to property.");   

//   // Update log
//   event_instance.update_log(rosmsg, msgInfo);

//   // Get log
//   updatedLog = event_instance.get_log();
//   currentRow = updatedLog[0];
  
//   // Check if content is equal
//   for(int idx = 1; idx < currentRow.size()-1; idx++){
//     ASSERT_EQ(event_details[idx-1], currentRow[idx]);    
//   }

//   // Clear event
//   event_instance.clear();
//   // Clear expected log
//   event_details.clear();
// }

TEST(RobotEventTestSuite, updateEventIdTest)
{
  // Sample message
  rcl_interfaces::msg::Log data;
  data.level = data.ERROR;
  data.name = source;
  data.msg = message;  
  rcl_interfaces::msg::Log::SharedPtr rosmsg(new rcl_interfaces::msg::Log(data));

  // For simple get test, no need for ECS 
  json::value msgInfo = json::value::null();

  // Declare log variable
  std::vector<std::vector<std::string>> updatedLog;
  std::vector<std::string> currentRow;

  // Update log
  event_instance.update_log(rosmsg, msgInfo);

  // Get log
  updatedLog = event_instance.get_log();
  
  // Get event id value
  currentRow = updatedLog[0];
  std::string eventId1 = currentRow.back();

  // Clear LOG and Update log again, this should result in same event id
  event_instance.clear_log();
  event_instance.update_log(rosmsg, msgInfo);

  // Get log
  updatedLog = event_instance.get_log();
  
  // Get event id value
  currentRow = updatedLog[0];
  std::string eventId2 = currentRow.back();

  // Clear EVENT and Update log again, this should result in a different event id
  event_instance.clear();
  event_instance.update_log(rosmsg, msgInfo);

  // Get log
  updatedLog = event_instance.get_log();
  
  // Get event id value
  currentRow = updatedLog[0];
  std::string eventId3 = currentRow.back();

  // Check if even ids 1 and 2 are equal
  ASSERT_EQ(eventId1, eventId2);

  // Check if even ids 2 and 3 are NOT equal
  ASSERT_NE(eventId2, eventId3);

  // Clear event
  event_instance.clear();
}

TEST(RobotEventTestSuite, clearTest)
{
  // Sample message
  rcl_interfaces::msg::Log data;
  data.level = data.ERROR;
  data.name = source;
  data.msg = message;  
  rcl_interfaces::msg::Log::SharedPtr rosmsg(new rcl_interfaces::msg::Log(data));

  // For simple get test, no need for ECS 
  json::value msgInfo = json::value::null();

  // Declare log variable
  std::vector<std::vector<std::string>> updatedLog;
  std::vector<std::string> currentRow;

  // Update log
  event_instance.update_log(rosmsg, msgInfo);

  // Get log
  updatedLog = event_instance.get_log();
  
  // Check log emptiness
  bool beforeClearEmptyFlag = updatedLog.empty();

  // Clear LOG 
  event_instance.clear_log();

  // Get log
  updatedLog = event_instance.get_log();

  // Check log emptiness
  bool afterClearEmptyFlag = updatedLog.empty();
  
  // Check if before clearing, log is NOT empty
  ASSERT_FALSE(beforeClearEmptyFlag);

  // Check if after clearing, log is empty
  ASSERT_TRUE(afterClearEmptyFlag);

  // Clear event
  event_instance.clear();
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
