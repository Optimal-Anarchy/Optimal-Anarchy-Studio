#pragma once

#include <stddef.h>
#include <imgui.h>

#ifdef IMNODES_USER_CONFIG
#include IMNODES_USER_CONFIG
#endif



#ifndef IMNODES_NAMESPACE
#define IMNODES_NAMESPACE ImNodes
#endif

typedef int ImNodesCol;             // -> enum ImNodesCol_
typedef int ImNodesStyleVar;        // -> enum ImNodesStyleVar_
typedef int ImNodesStyleFlags;      // -> enum ImNodesStyleFlags_
typedef int ImNodesPinShape;        // -> enum ImNodesPinShape_
typedef int ImNodesAttributeFlags;  // -> enum ImNodesAttributeFlags_
typedef int ImNodesMiniMapLocation; // -> enum ImNodesMiniMapLocation_

enum ImNodesCol_
{
    ImNodesCol_NodeBackground = 0,
    ImNodesCol_NodeBackgroundHovered,
    ImNodesCol_NodeBackgroundSelected,
    ImNodesCol_NodeOutline,
    ImNodesCol_TitleBar,
    ImNodesCol_TitleBarHovered,
    ImNodesCol_TitleBarSelected,
    ImNodesCol_Link,
    ImNodesCol_LinkHovered,
    ImNodesCol_LinkSelected,
    ImNodesCol_Pin,
    ImNodesCol_PinHovered,
    ImNodesCol_BoxSelector,
    ImNodesCol_BoxSelectorOutline,
    ImNodesCol_GridBackground,
    ImNodesCol_GridLine,
    ImNodesCol_GridLinePrimary,
    ImNodesCol_MiniMapBackground,
    ImNodesCol_MiniMapBackgroundHovered,
    ImNodesCol_MiniMapOutline,
    ImNodesCol_MiniMapOutlineHovered,
    ImNodesCol_MiniMapNodeBackground,
    ImNodesCol_MiniMapNodeBackgroundHovered,
    ImNodesCol_MiniMapNodeBackgroundSelected,
    ImNodesCol_MiniMapNodeOutline,
    ImNodesCol_MiniMapLink,
    ImNodesCol_MiniMapLinkSelected,
    ImNodesCol_MiniMapCanvas,
    ImNodesCol_MiniMapCanvasOutline,
    ImNodesCol_COUNT
};

enum ImNodesStyleVar_
{
    ImNodesStyleVar_GridSpacing = 0,
    ImNodesStyleVar_NodeCornerRounding,
    ImNodesStyleVar_NodePadding,
    ImNodesStyleVar_NodeBorderThickness,
    ImNodesStyleVar_LinkThickness,
    ImNodesStyleVar_LinkLineSegmentsPerLength,
    ImNodesStyleVar_LinkHoverDistance,
    ImNodesStyleVar_PinCircleRadius,
    ImNodesStyleVar_PinQuadSideLength,
    ImNodesStyleVar_PinTriangleSideLength,
    ImNodesStyleVar_PinLineThickness,
    ImNodesStyleVar_PinHoverRadius,
    ImNodesStyleVar_PinOffset,
    ImNodesStyleVar_MiniMapPadding,
    ImNodesStyleVar_MiniMapOffset,
    ImNodesStyleVar_COUNT
};

enum ImNodesStyleFlags_
{
    ImNodesStyleFlags_None = 0,
    ImNodesStyleFlags_NodeOutline = 1 << 0,
    ImNodesStyleFlags_GridLines = 1 << 2,
    ImNodesStyleFlags_GridLinesPrimary = 1 << 3,
    ImNodesStyleFlags_GridSnapping = 1 << 4
};

enum ImNodesPinShape_
{
    ImNodesPinShape_Circle,
    ImNodesPinShape_CircleFilled,
    ImNodesPinShape_Triangle,
    ImNodesPinShape_TriangleFilled,
    ImNodesPinShape_Quad,
    ImNodesPinShape_QuadFilled
};

// This enum controls the way the attribute pins behave.
enum ImNodesAttributeFlags_
{
    ImNodesAttributeFlags_None = 0,
    // Allow detaching a link by left-clicking and dragging the link at a pin it is connected to.
    // NOTE: the user has to actually delete the link for this to work. A deleted link can be
    // detected by calling IsLinkDestroyed() after EndNodeEditor().
    ImNodesAttributeFlags_EnableLinkDetachWithDragClick = 1 << 0,
    // Visual snapping of an in progress link will trigger IsLink Created/Destroyed events. Allows
    // for previewing the creation of a link while dragging it across attributes. See here for demo:
    // https://github.com/Nelarius/imnodes/issues/41#issuecomment-647132113 NOTE: the user has to
    // actually delete the link for this to work. A deleted link can be detected by calling
    // IsLinkDestroyed() after EndNodeEditor().
    ImNodesAttributeFlags_EnableLinkCreationOnSnap = 1 << 1
};

struct ImNodesIO
{
    struct EmulateThreeButtonMouse
    {
        EmulateThreeButtonMouse();

        // The keyboard modifier to use in combination with mouse left click to pan the editor view.
        // Set to NULL by default. To enable this feature, set the modifier to point to a boolean
        // indicating the state of a modifier. For example,
        //
        // ImNodes::GetIO().EmulateThreeButtonMouse.Modifier = &ImGui::GetIO().KeyAlt;
        const bool* Modifier;
    } EmulateThreeButtonMouse;

    struct LinkDetachWithModifierClick
    {
        LinkDetachWithModifierClick();

        // Pointer to a boolean value indicating when the desired modifier is pressed. Set to NULL
        // by default. To enable the feature, set the modifier to point to a boolean indicating the
        // state of a modifier. For example,
        //
        // ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
        //
        // Left-clicking a link with this modifier pressed will detach that link. NOTE: the user has
        // to actually delete the link for this to work. A deleted link can be detected by calling
        // IsLinkDestroyed() after EndNodeEditor().
        const bool* Modifier;
    } LinkDetachWithModifierClick;

    struct MultipleSelectModifier
    {
        MultipleSelectModifier();

        // Pointer to a boolean value indicating when the desired modifier is pressed. Set to NULL
        // by default. To enable the feature, set the modifier to point to a boolean indicating the
        // state of a modifier. For example,
        //
        // ImNodes::GetIO().MultipleSelectModifier.Modifier = &ImGui::GetIO().KeyCtrl;
        //
        // Left-clicking a node with this modifier pressed will add the node to the list of
        // currently selected nodes. If this value is NULL, the Ctrl key will be used.
        const bool* Modifier;
    } MultipleSelectModifier;

    // Holding alt mouse button pans the node area, by default middle mouse button will be used
    // Set based on ImGuiMouseButton values
    int AltMouseButton;

    // Panning speed when dragging an element and mouse is outside the main editor view.
    float AutoPanningSpeed;

    ImNodesIO();
};

struct ImNodesStyle
{
    float GridSpacing;

    float  NodeCornerRounding;
    ImVec2 NodePadding;
    float  NodeBorderThickness;

    float LinkThickness;
    float LinkLineSegmentsPerLength;
    float LinkHoverDistance;

    // The following variables control the look and behavior of the pins. The default size of each
    // pin shape is balanced to occupy approximately the same surface area on the screen.

    // The circle radius used when the pin shape is either ImNodesPinShape_Circle or
    // ImNodesPinShape_CircleFilled.
    float PinCircleRadius;
    // The quad side length used when the shape is either ImNodesPinShape_Quad or
    // ImNodesPinShape_QuadFilled.
    float PinQuadSideLength;
    // The equilateral triangle side length used when the pin shape is either
    // ImNodesPinShape_Triangle or ImNodesPinShape_TriangleFilled.
    float PinTriangleSideLength;
    // The thickness of the line used when the pin shape is not filled.
    float PinLineThickness;
    // The radius from the pin's center position inside of which it is detected as being hovered
    // over.
    float PinHoverRadius;
    // Offsets the pins' positions from the edge of the node to the outside of the node.
    float PinOffset;

    // Mini-map padding size between mini-map edge and mini-map content.
    ImVec2 MiniMapPadding;
    // Mini-map offset from the screen side.
    ImVec2 MiniMapOffset;

    // By default, ImNodesStyleFlags_NodeOutline and ImNodesStyleFlags_Gridlines are enabled.
    ImNodesStyleFlags Flags;
    // Set these mid-frame using Push/PopColorStyle. You can index this color array with with a
    // ImNodesCol value.
    unsigned int Colors[ImNodesCol_COUNT];

    ImNodesStyle();
};

enum ImNodesMiniMapLocation_
{
    ImNodesMiniMapLocation_BottomLeft,
    ImNodesMiniMapLocation_BottomRight,
    ImNodesMiniMapLocation_TopLeft,
    ImNodesMiniMapLocation_TopRight,
};

struct ImGuiContext;
struct ImVec2;

struct ImNodesContext;

// An editor context corresponds to a set of nodes in a single workspace (created with a single
// Begin/EndNodeEditor pair)
//
// By default, the library creates an editor context behind the scenes, so using any of the imnodes
// functions doesn't require you to explicitly create a context.
struct ImNodesEditorContext;

// Callback type used to specify special behavior when hovering a node in the minimap
#ifndef ImNodesMiniMapNodeHoveringCallback
typedef void (*ImNodesMiniMapNodeHoveringCallback)(int, void*);
#endif

#ifndef ImNodesMiniMapNodeHoveringCallbackUserData
typedef void* ImNodesMiniMapNodeHoveringCallbackUserData;
#endif

namespace ImNodes {
    IMGUI_API void SetImGuiContext(ImGuiContext* ctx);

    IMGUI_API ImNodesContext* CreateContext();
    IMGUI_API void DestroyContext(ImNodesContext* ctx = NULL); // NULL = destroy current context
    IMGUI_API ImNodesContext* GetCurrentContext();
    IMGUI_API void SetCurrentContext(ImNodesContext* ctx);

    IMGUI_API ImNodesEditorContext* EditorContextCreate();
    IMGUI_API void EditorContextFree(ImNodesEditorContext*);
    IMGUI_API void EditorContextSet(ImNodesEditorContext*);
    IMGUI_API ImVec2 EditorContextGetPanning();
    IMGUI_API void EditorContextResetPanning(const ImVec2& pos);
    IMGUI_API void EditorContextMoveToNode(const int node_id);

    IMGUI_API ImNodesIO& GetIO();

    IMGUI_API ImNodesStyle& GetStyle();
    IMGUI_API void StyleColorsDark(ImNodesStyle* dest = NULL); // on by default
    IMGUI_API void StyleColorsClassic(ImNodesStyle* dest = NULL);
    IMGUI_API void StyleColorsLight(ImNodesStyle* dest = NULL);

    IMGUI_API void BeginNodeEditor();
    IMGUI_API void EndNodeEditor();

    IMGUI_API void MiniMap(
        const float                                      minimap_size_fraction = 0.2f,
        const ImNodesMiniMapLocation                     location = ImNodesMiniMapLocation_TopLeft,
        const ImNodesMiniMapNodeHoveringCallback         node_hovering_callback = NULL,
        const ImNodesMiniMapNodeHoveringCallbackUserData node_hovering_callback_data = NULL);

    IMGUI_API void PushColorStyle(ImNodesCol item, unsigned int color);
    IMGUI_API void PopColorStyle();
    IMGUI_API void PushStyleVar(ImNodesStyleVar style_item, float value);
    IMGUI_API void PushStyleVar(ImNodesStyleVar style_item, const ImVec2& value);
    IMGUI_API void PopStyleVar(int count = 1);

    IMGUI_API void BeginNode(int id);
    IMGUI_API void EndNode();

    IMGUI_API ImVec2 GetNodeDimensions(int id);

    IMGUI_API void BeginNodeTitleBar();
    IMGUI_API void EndNodeTitleBar();

    IMGUI_API void BeginInputAttribute(int id, ImNodesPinShape shape = ImNodesPinShape_CircleFilled);
    IMGUI_API void EndInputAttribute();
    IMGUI_API void BeginOutputAttribute(int id, ImNodesPinShape shape = ImNodesPinShape_CircleFilled);
    IMGUI_API void EndOutputAttribute();
    IMGUI_API void BeginStaticAttribute(int id);
    IMGUI_API void EndStaticAttribute();

    IMGUI_API void PushAttributeFlag(ImNodesAttributeFlags flag);
    IMGUI_API void PopAttributeFlag();

    IMGUI_API void Link(int id, int start_attribute_id, int end_attribute_id);

    IMGUI_API void SetNodeDraggable(int node_id, const bool draggable);

    IMGUI_API void SetNodeScreenSpacePos(int node_id, const ImVec2& screen_space_pos);
    IMGUI_API void SetNodeEditorSpacePos(int node_id, const ImVec2& editor_space_pos);
    IMGUI_API void SetNodeGridSpacePos(int node_id, const ImVec2& grid_pos);

    IMGUI_API ImVec2 GetNodeScreenSpacePos(const int node_id);
    IMGUI_API ImVec2 GetNodeEditorSpacePos(const int node_id);
    IMGUI_API ImVec2 GetNodeGridSpacePos(const int node_id);

    IMGUI_API void SnapNodeToGrid(int node_id);

    IMGUI_API bool IsEditorHovered();
    IMGUI_API bool IsNodeHovered(int* node_id);
    IMGUI_API bool IsLinkHovered(int* link_id);
    IMGUI_API bool IsPinHovered(int* attribute_id);

    IMGUI_API int NumSelectedNodes();
    IMGUI_API int NumSelectedLinks();
    IMGUI_API void GetSelectedNodes(int* node_ids);
    IMGUI_API void GetSelectedLinks(int* link_ids);
    IMGUI_API void ClearNodeSelection();
    IMGUI_API void ClearLinkSelection();
    IMGUI_API void SelectNode(int node_id);
    IMGUI_API void ClearNodeSelection(int node_id);
    IMGUI_API bool IsNodeSelected(int node_id);
    IMGUI_API void SelectLink(int link_id);
    IMGUI_API void ClearLinkSelection(int link_id);
    IMGUI_API bool IsLinkSelected(int link_id);

    IMGUI_API bool IsAttributeActive();
    IMGUI_API bool IsAnyAttributeActive(int* attribute_id = NULL);

    IMGUI_API bool IsLinkStarted(int* started_at_attribute_id);
    IMGUI_API bool IsLinkDropped(int* started_at_attribute_id = NULL, bool including_detached_links = true);
    IMGUI_API bool IsLinkCreated(
        int* started_at_attribute_id,
        int* ended_at_attribute_id,
        bool* created_from_snap = NULL);
    IMGUI_API bool IsLinkCreated(
        int* started_at_node_id,
        int* started_at_attribute_id,
        int* ended_at_node_id,
        int* ended_at_attribute_id,
        bool* created_from_snap = NULL);

    IMGUI_API bool IsLinkDestroyed(int* link_id);

    IMGUI_API const char* SaveCurrentEditorStateToIniString(size_t* data_size = NULL);
    IMGUI_API const char* SaveEditorStateToIniString(
        const ImNodesEditorContext* editor,
        size_t* data_size = NULL);

    IMGUI_API void LoadCurrentEditorStateFromIniString(const char* data, size_t data_size);
    IMGUI_API void LoadEditorStateFromIniString(ImNodesEditorContext* editor, const char* data, size_t data_size);

    IMGUI_API void SaveCurrentEditorStateToIniFile(const char* file_name);
    IMGUI_API void SaveEditorStateToIniFile(const ImNodesEditorContext* editor, const char* file_name);

    IMGUI_API void LoadCurrentEditorStateFromIniFile(const char* file_name);
    IMGUI_API void LoadEditorStateFromIniFile(ImNodesEditorContext* editor, const char* file_name);
} // namespace ImNodes

