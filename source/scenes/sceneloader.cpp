#include "sceneloader.h"
#include "base64.hpp"
#include "componentmanager.h"
#include "console.h"
#include "defines.h"
#include "gameobject.h"
#include "ql_assert.h"
#include "readfile.h"
#include "scene.h"
#include "scenemanager.h"
#include "slmdlloader.h"
#include <3ds.h>
#include <cctype>
#include <fstream>
#include <memory>
#include <stdlib.h>
#include <string>
#include <string_view>

namespace ql {
	// helper functions for the loader
	namespace {
		struct sceneLoadThreadParams {
			std::unique_ptr<Scene> s; // scene pointer we are writing to
			std::string name;		  // scene file name
			float progress;			  // progress value callback
			bool isdone;
			bool activate;
			LightEvent event;
		};

		void remove_whitespace(auto &str) {
			char *pc = str.get(), *pd = pc;
			do {
				while (std::isspace(*pc)) // skip whitespace
					pc++;
			} while ((*pd++ = *pc++)); // copy over
		}
	} // namespace

	void sceneLoadThread(void *params) {

		if (!params) {
			Console::error("Params are null");
			return;
		}

		sceneLoadThreadParams &p =
			*(sceneLoadThreadParams *)params; // reference to not make a copy

		LightEvent_Init(&p.event, RESET_ONESHOT);

		auto textstr{readFileAligned(("romfs:/scenes/" + p.name + ".scene"))};
		ASSERT(textstr != nullptr, "Invalid scene file");

		remove_whitespace(textstr);
		std::string_view text{textstr.get()};

		
		p.s->root = SceneLoader::parseObjectAsync(
			p.s, text, text.size(),
			p.progress); // parse the whole object tree
		p.progress = 1;
		p.isdone   = true;


		Console::success("finished loading scene file");

		if (!p.activate)
			LightEvent_Wait(&p.event); // if the user doesn't want it activated
									   // immediately, wait for their signal

		SceneManager::setScene(p.s);

		delete (sceneLoadThreadParams *)params; // clean up the pointer
	}

	AsyncSceneLoadOperation SceneLoader::loadAsync(std::string name) {
		// check if scene file exists
		std::ifstream scenefile;
		scenefile.open(("romfs:/scenes/" + name + ".scene"));
		assert(scenefile.is_open());
		scenefile.close();

		// start of scene load async function

		// needs to be a raw pointer otherwise I would make it a smart pointer
		sceneLoadThreadParams *p =
			new sceneLoadThreadParams{std::make_unique<Scene>(name),
									  name,
									  0.f,
									  false,
									  true,
									  LightEvent()};

		// Set the thread priority to the main thread's priority ...
		int32_t priority = 0x30;
		svcGetThreadPriority(&priority, CUR_THREAD_HANDLE);
		// ... then add 1, as higher number => lower actual priority ...
		priority += 1;
		// ... finally, clamp it between 0x18 and 0x3F to guarantee that it's
		// valid.
		priority = priority < 0x18 ? 0x18 : priority;
		priority = priority > 0x3F ? 0x3F : priority;

		// Start the thread
		threadCreate(sceneLoadThread, p, SCENELOADER_THREAD_STACK_SZ, priority,
					 -1, true);

		return AsyncSceneLoadOperation{
			&p->progress, &p->isdone,
			&p->event}; // changed to shared/weak ptr for safety
	}

	bool SceneLoader::load(std::string name) {
		std::unique_ptr<Scene> out = std::make_unique<Scene>(name);

		auto textstr{readFileAligned(("romfs:/scenes/" + name + ".scene"))};
		ASSERT(textstr != nullptr, "Invalid scene file");
		remove_whitespace(textstr);
		std::string_view text{textstr.get()};
		Console::success("read scene file");

		// parse the whole object tree recursively
		out->root = parseObject(out, text);
		Console::success("finished parsing scene file");

		SceneManager::setScene(out);
		return true;
	}

	void SceneLoader::printSceneTree(std::shared_ptr<GameObject> &root, int indentlevel) {
		Console::log("%.*s%s", indentlevel * 2,
					 "| | | | | | | | | | | | | | | | | | | | | | | | | | | | ",
					 root->name.c_str());
		for (auto obj : root->children) {
			if (!obj)
				continue; // make sure children aren't empty references
			printSceneTree(obj, indentlevel + 1);
		}
	}

	void SceneLoader::parseChildren(std::unique_ptr<Scene> &s,
									std::shared_ptr<GameObject> object,
									std::string_view &input) {
		while (input[0] != ']' && input.size() > 0) {
			object->assignChild(parseObject(s, input));
		}
		input.remove_prefix(input.find(']') + 1);
		if (input[0] == ',')
			input.remove_prefix(1);
	}

	void SceneLoader::parseChildrenAsync(std::unique_ptr<Scene> &s,
										 std::shared_ptr<GameObject>object,
										 std::string_view &input,
										 unsigned int size, float &progress) {
		while (input[0] != ']' && input.size() > 0) {
			object->assignChild(parseObjectAsync(s, input, size, progress));
		}
		input.remove_prefix(input.find(']') + 1);
		if (input[0] == ',')
			input.remove_prefix(1);
	}

	std::shared_ptr<GameObject> SceneLoader::parseObject(std::unique_ptr<Scene> &s,
								  std::string_view &input) {
		auto object = std::make_shared<GameObject>(std::string{input.substr(0, input.find('['))}, *s.get());

		input.remove_prefix(input.find('[') + 1); // go to start of objects

		for (int i = 0; i < 3; i++) {
			std::string attr{input.substr(
				0, input.find('['))}; // isolate the name of the first part
			input.remove_prefix(input.find('[') +
								1); // go to start of that section

			if (attr == "components")
				parseComponents(s, object, input);
			else if (attr == "scripts")
				parseScripts(s, object, input);
			else if (attr == "children")
				parseChildren(s, object, input);
			else
				break;
		}

		input.remove_prefix(input.find(']') + 1);
		if (input[0] == ',')
			input.remove_prefix(1);
		return object;
	}

	std::shared_ptr<GameObject> SceneLoader::parseObjectAsync(std::unique_ptr<Scene> &s,
									   std::string_view &input,
									   unsigned int size, float &progress) {
		THREAD_YIELD; // yield thread so the loader doesn't hog the cpu
		auto object = std::make_shared<GameObject>(std::string{input.substr(0, input.find('['))},
*s.get());
		input.remove_prefix(input.find('[') + 1); // go to start of objects
		for (int i = 0; i < 3; i++) {
			std::string attr{input.substr(
				0, input.find('['))}; // isolate the name of the first part
			input.remove_prefix(input.find('[') +
								1); // go to start of that section

			// allows any order, as well as omission
			if (attr == "components")
				parseComponents(s, object, input);
			else if (attr == "scripts")
				parseScripts(s, object, input);
			else if (attr == "children")
				parseChildrenAsync(s, object, input, size, progress);
			else
				break;
		}

		input.remove_prefix(input.find(']') + 1);
		if (input[0] == ',')
			input.remove_prefix(1);

		progress = 1 - (((float)input.size()) / size);
		printf("\e[s\e[24;%uH%c\e[u", (int)size * 40, '-');
		// Console::log("Progress %f", progress);
		return object;
	}

	void SceneLoader::parseScripts(std::unique_ptr<Scene> &s,
								   std::shared_ptr<GameObject>object,
								   std::string_view &input) {
		while (input[0] != ']') { // ] means end of the section
			if (input.find(',') < input.find(']')) { // this means there is a , closer than a ] so there must be at least another value
				ComponentManager::addScript(std::string(input.substr(0, input.find(']'))).c_str(), object);
				input.remove_prefix(input.find(',') + 1); // go to next one
			} else {									  // this means there is only one in there
				ComponentManager::addScript(std::string(input.substr(0, input.find(']'))).c_str(), object);
				input.remove_prefix(input.find(']') +
									1); // go to the end of the section
				break;
			}
		}
		input.remove_prefix(input.find(',') + 1); // go to end of section
	}

	void SceneLoader::parseComponents(std::unique_ptr<Scene> &s,
									  std::shared_ptr<GameObject> object,
									  std::string_view &input) {
		while (input[0] != ']') { // ] means end of the section
			if (input.find(',') <
				input.find(']')) { // this means there is a , closer than a ] so
								   // there must be at least another value
				parseComponent(s, object, input.substr(0, input.find(',')));
				input.remove_prefix(input.find(',') + 1); // go to next one
			} else {									  // this means there is only one in there
				parseComponent(s, object, input.substr(0, input.find(']')));
				input.remove_prefix(input.find(']') +
									1); // go to the end of the section
				break;
			}
		}

		input.remove_prefix(input.find(',') + 1);
	}

	void SceneLoader::parseComponent(std::unique_ptr<Scene> &s,
									 std::shared_ptr<GameObject> object,
									 std::string_view input) {
		std::string componentname{input.substr(0, input.find('{'))};

		// deserialize base64 data
		const void *bindata = NULL;
		std::string base64data;
		if (input.find('}') - input.find('{') - 1 > 0) {
			base64data = base64::from_base64(input.substr(
							 input.find('{') + 1,
							 input.find('}') - input.find('{') - 1)) +
						 '\0';
			bindata = base64data.c_str(); // get pointer to contained data
		}

		ComponentManager::addComponent(componentname.c_str(), object, bindata);
	}
} // namespace ql
