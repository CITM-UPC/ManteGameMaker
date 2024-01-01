#pragma once

class EditorModule {
public:
	EditorModule() {}

	~EditorModule() {}

	virtual bool Start() { return true; }
	virtual bool PreUpdate() { return true; }
	virtual bool Update() { return true; }
	virtual bool PostUpdate() { return true; }
	virtual bool CleanUp() { return true; }

};