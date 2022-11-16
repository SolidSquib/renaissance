#pragma once

#include "Renaissance.h"
#include "Faireground/Windows/EditorWindow.h"

#include <filesystem>

namespace Renaissance
{
	class ContentBrowserWindow : public EditorWindow
	{
	public:
		ContentBrowserWindow(uint32_t index);

		virtual void OnUIRender() override;
		virtual bool WantsToClose() const override { return !mOpen; }

		void Open() override;
		void Close() override;

		void RefreshFolderView(const std::filesystem::path& newPath);
		bool DrawBrowserSettings();

		void DrawIconForPath(const std::filesystem::path& path, bool asListItem);
		String GetDragContext(const std::filesystem::path& path);

	private:
		bool mOpen = true;
		bool mRefreshFolders = true;

		uint32_t mContentBrowserIndex = 0;
		String mContentBrowserName = "Content Browser";
		std::filesystem::path mActivePath;

		int mIconSize = 35;
		int mIconPadding = 25;

		std::vector<std::filesystem::path> mActivePathDirectories;
		std::vector<std::filesystem::path> mActivePathFiles;

		static ImFont* IconFontLarge;
	};
}