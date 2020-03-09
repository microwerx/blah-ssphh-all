#ifndef SSPHH_HPP
#define SSPHH_HPP

#include <AuroraVisualization.hpp>
#include <PbskyViewController.hpp>
#include <camera_animation.hpp>
#include <damselfish_python.hpp>
#include <fluxions.hpp>
#include <fluxions_sphl.hpp>
#include <fluxions_ssg_ssphh.hpp>
#include <fluxions_ssg_ssphh_renderer_plugin.hpp>
#include <ssphh_ppmcompare.hpp>
#include <ssphh_unicornfish.hpp>
#include <viperfish_dear_imgui.hpp>
#include <viperfish_mouse.hpp>
#include <viperfish_widget.hpp>

using namespace Vf;
using namespace Fluxions;

namespace SSPHH {
	extern const char* default_renderconfig_path;
	extern const char* default_scene_graph_path;
	extern const std::string default_coronaskyboxcubemap_path;
	extern const std::string default_pbsky_cubemap1_ppm;
	extern const std::string default_pbsky_cubemap2_ppm;
	extern const std::string default_pbsky_cubemap3_ppm;
	extern const std::string default_pbsky_cubemap4_ppm;
	extern const std::string default_pbsky_cubemap5_ppm;
	extern const std::string default_pbsky_cubemap6_ppm;
	extern const std::string default_pbsky_cylmap_ppm;
	extern const std::string default_pbsky_cylmap_exr;
	extern const std::string default_pbsky_cubemap_ppm;
	extern const std::string default_pbsky_cubemap_exr;
} // namespace SSPHH

// extern RendererWindowPtr renderer_window_ptr;
// extern RendererConfigWindowPtr renderer_config_window_ptr;

namespace SSPHH {
	class SSPHH_Application : public Widget {
	public:
		// PHYSICALLY BASED SKY METHODS/MEMBERS /////////////////////////////

		// Physically Based Code
		// Color4f groundRadiance;
		// Color4f sunDiskRadiance;
		bool pbskyReuseCorona{ false };
		double pbskyAge = 0.0;
		double pbskyMaxAge = 5.0;
		// double pbskyLatitude = 38.9;
		// double pbskyLongitude = -104.8;
		// bool recomputeSkyCube;
		bool isSkyComputing;
		bool isSkyComputed;
		time_t pbsky_localtime;
		double pbsky_timeOffsetInSeconds = 0.0;
		BaseDirToLight* sun{ nullptr };
		BaseDirToLight* moon{ nullptr };
		SimpleGeometryGroupPtr moonGG;
		// void pbskyComputeCubemap();
		// RendererTextureObject PBSkyCubeMap;
		SimpleRenderer_GLushort Skybox;
		void Sky_RegenHosekWilkieTextures();
		void Sky_SaveHosekWilkieTextures();
		void Sky_RegenCoronaSky();
		void Sky_LoadCoronaSky(bool loadEXR, const std::string& path);
		void Sky_InitViewController();
		void Sky_Render();

		AuroraViz auroraViz;

		///////////////////////////////////////////////////////////////

	public:
		// GLfloat aspect = 1.0f;
		// GLfloat fovy = 45.0f;
		//// GLfloat nearVal = 0.001f;
		//// GLfloat farVal = 1000.0f;
		// GLfloat screenX = 0.0f;
		// GLfloat screenY = 0.0f;
		// GLfloat screenWidth = 0.0f;
		// GLfloat screenHeight = 0.0f;

		// SCENE GRAPH METHODS

		float rotX = 0.0;
		float rotY = 0.0;
		float rotZ = 0.0;

		// std::string sceneFilename = "resources/scenes/test_texture_scene/test_outdoor_scene.scn";
		// std::string sceneFilename = "resources/scenes/test_texture_scene/test_mitsuba_scene.scn";
		// std::string sceneFilename = "resources/scenes/test_texture_scene/test_indoor_scene.scn";
		std::string sceneFilename = "resources/scenes/test_texture_scene/test_gallery_scene.scn";
		// std::string sceneFilename = "resources/scenes/test_texture_scene/simple_inside_scene.scn";
		// std::string sceneFilename = "resources/scenes/rungholt_scene/rungholt.scn";

		// Uf::CoronaDatabase sceneDB;
		IBaseObjectPtr root;
		Fluxions::SimpleSceneGraphPtr ssg;
		Fluxions::SimpleSSPHH ssphh;
		Fluxions::SSG_SSPHHRendererPluginPtr ssgUserData;
		Uf::CoronaSceneFile coronaScene;
		std::vector<Uf::CoronaJob> coronaJobs;

		void SSG_LoadScene();
		void SSG_ReloadScene();
		void SSG_OptimizeClippingPlanes();
		void SSG_ProcessInterfaceTasks();

		// GRAPHICAL USER INTERFACE METHODS

		struct InterfaceInfo {
			Matrix4f projectionMatrix;
			Matrix4f preCameraMatrix;
			Matrix4f inversePreCameraMatrix;
			Matrix4f postCameraMatrix;
			Matrix4f inversePostCameraMatrix;
			Matrix4f finalCameraMatrix;
			Matrix4f inverseFinalCameraMatrix;
			Vector4f cameraPosition;
			Vector4f cameraOrbit;

			double moveX = 0.0;
			double moveY = 0.0;
			double moveZ = 0.0;
			double turnX = 0.0;
			double turnY = 0.0;
			double turnZ = 0.0;
			double moveRate = 0.5;
			double turnRate = 1.0;

			bool enableAnimation = true;
			bool enableOrbit = false;
			bool enableSunCycle = false;
			bool usePBSKY = true;
			bool drawSkyBox = true;
			bool drawPBR = true;
			bool drawVIZ = true;
			bool drawPOST = true;
			bool saveScreenshot = false;
			bool captureShadows = false;

			bool moveLeft = false;
			bool moveRight = false;
			bool moveForward = false;
			bool moveBackward = false;
			bool moveDown = false;
			bool moveUp = false;
			bool yawLeft = false;
			bool yawRight = false;
			bool pitchDown = false;
			bool pitchUp = false;
			bool rollLeft = false;
			bool rollRight = false;

			bool increaseLatitude = false;
			bool increaseLongitude = false;
			bool decreaseLatitude = false;
			bool decreaseLongitude = false;

			bool showHelp = false;
			bool showHUD = false;
			bool showMainHUD = false;
			bool showDeferredHUD = false;
			bool showImGui = false;

			struct TOOLSINFO {
				bool showMaterialEditor = false;
				bool showSphlEditor = false;
				bool showScenegraphEditor = false;
				bool showDebugView = false;
				bool showSSPHHWindow = true;
				bool showTestsWindow = true;
				bool showUnicornfishWindow = true;
				bool showRenderConfigWindow = false;
				int shaderDebugChoice = 17;
				int shaderDebugLight = -1;
				int shaderDebugSphl = -1;
				int shaderDebugChoices[4] = { 0, 0, 0, 0 };
				std::vector<std::string> gl_extensions;
				std::vector<const char*> gl_extensions_cstr;
				int gl_extensions_curitem = 0;
				std::string gl_version;
				std::string gl_renderer;
				bool showGLInfo = false;
			} tools;

			struct SSGINFO {
				bool showEnvironment = false;
				bool showEnvironmentDetails = false;
				bool showGeometry = false;
				bool showGeometryDetails = false;
				bool showSpheres = false;
				bool showSphereDetails = false;
				bool showPointLights = false;
				bool showPointLightDetails = false;

				float cameraFOV = 45.0f;

				std::vector<std::pair<int, bool>> geometryCollapsed;

				std::string scenename;

				bool resetScene = false;
				bool saveScene = false;
				bool loadScene = false;
				bool createScene = false;
			} ssg;

			struct MATERIALSINFO {
				struct MtlValuePtrs {
					float* PBm = nullptr;
					float* PBk = nullptr;
					float* PBior = nullptr;
					float* PBKdm = nullptr;
					float* PBKsm = nullptr;
					float* PBGGXgamma = nullptr;
					float* Kd = nullptr;
					float* Ks = nullptr;
					float* Ke = nullptr;
					float* Ka = nullptr;
					const char* map_Kd = nullptr;
					const char* map_Ks = nullptr;
					const char* map_normal = nullptr;
					const char* map_bump = nullptr;
				};
				bool showMaps = false;
				bool showMtls = true;
				std::map<std::string, bool> mtllibsCollapsed;
				std::map<std::string, bool> mtlsCollapsed;
				std::map<std::string, SimpleMaterial*> mtls;
				std::map<std::string, MtlValuePtrs> mtls_ptrs;
				// vector<pair<bool, vector<int, bool>>> mtlsCollapsed;
			} mtls;

			struct SSPHHWINDOW {
				bool enabled = false;
				static const int MaxSPHLs = 16;

				enum class SSPHH_Status { SendUfMessage = 0, RecvUfMessage = 1, UploadToGpu = 2, Finished };

				int numSphls = 0;
				std::map<int, SSPHH_Status> ssphhStatus;
				bool enableShadowColorMap = false;
				bool enableShowHierarchies = false;
				bool enableShowSPHLs = true;
				bool enableBasicShowSPHLs = true;
				std::vector<double> gen_times;
				std::vector<double> viz_times;
				bool HierarchiesIncludeSelf = true;
				bool HierarchiesIncludeNeighbors = true;
				float HierarchiesMix{ 0 };
				int HierarchiesMaxSphls = MaxSphlLights;
				bool HierarchiesGeneratePPMs = false;
				int MaxDegrees = MaxSphlDegree;

				int LightProbeSizeChoice = 4;
				int LightProbeSize = 2 << 4;
				int ShadowSizeChoice = 6;
				int ShadowSize = 2 << 6;
				bool genProductsIgnoreCache = false;
				bool ppmcompareIgnoreCache = false;
				bool ppmcompareGenPPMs = false;
				std::string gi_status;

				bool enableREF = true;
				bool enableREFCubeMap = false;

				std::string lastAPPImagePath;		 // The previous screenshot
				std::string lastREFImagePath;		 // The previous ground truth
				std::string lastREFCubeMapImagePath; // The previous ground truth cube
				std::string lastGENRenderImagePath;	 // The previous gen light probe
				std::string lastVIZRenderImagePath;	 // The previous viz light probe

				int cacheFilesRemoved{ 0 };
				double lastAPPTime = 0.0;
				double lastREFTime = 0.0;
				double lastREFCubeMapTime = 0.0;
				double lastINITTime = 0.0;
				double lastVIZTime = 0.0;
				double lastGENTime = 0.0;
				double lastHIERTime = 0.0;
				double lastPathTracerTotalEnergy = 0.0;
				double lastSphlRenderTotalEnergy = 0.0;
				double lastDiff1TotalEnergy = 0.0;
				double lastDiff2TotalEnergy = 0.0;
			} ssphh;

			struct UNICORNFISHWINDOW {
				// Uf read from variables / UI write to variables
				std::vector<std::string> send_queue;
				std::vector<const char*> send_queue_items;
				int send_queue_item = 0;

				// Uf write to variables / UI read from variables
				std::vector<std::string> recv_queue;
				std::vector<const char*> recv_queue_items;
				int recv_queue_item = 0;

				bool uf_isinit = false; // one time flag to tell service if it has started or not
				bool uf_stop = false;	// one time flag to tell service to stop
				UfType uf_type{ UfType::None };
				std::string endpoint{ "" };
				std::string service{ "" };

				bool standalone_client = true;
				bool standalone_broker = true;
				bool standalone_worker = true;

				std::thread broker_thread;
				std::thread worker_thread;
				std::thread client_thread;

				bool windowInit = false;
			} uf;

			struct RENDERCONFIGWINDOW {
				int sunShadowMapSizeChoice = 10;
				int sunShadowMapSize = 2 << 10;
				float sunShadowMapZoom = 1.0;
			} renderconfig;

			struct TESTSWINDOW {
				bool bTestSPHLs = false;
				bool bShowSPHLResults = false;
				int saveSphlJSON = 0;
				int readSphlJSON = 0;
				int saveSphlOBJ = 0;
				int saveSphlEXR = 0;
				int readSphlEXR = 0;
				int saveSphlPPM = 0;
				int readSphlPPM = 0;
			} tests;

			bool recomputeSky = true;

			bool saveCoronaSCN = false;
			bool saveCoronaCubeMapSCN = false;
			bool renderCoronaSCN = false;
			bool renderCoronaCubeMapSCN = false;

			bool sphl_checkboxes[32][4] = { { false } };

			double lastRenderConfigLoadTime = 0.0;
			double lastScenegraphLoadTime = 0.0;

			std::string sceneName;
		}; // InterfaceInfo

		float imguiWinX = 64.0f;
		float imguiWinW = 384.0f;

		struct HUDInfo {
			double pbskyTime = 0.0;
			double gbufferPassTime = 0.0;
			double deferredPassTime = 0.0;
			double totalRenderTime = 0.0;
			double onUpdateTime = 0.0;

			std::string glRendererString = "";
			std::string glVersionString = "";
			std::string glVendorString = "";
			std::vector<std::string> glLastDebugMessages;
		};

		void InitImGui();

		void imguiSphlAdd();
		void imguiSphlDelete();
		void imguiSphlRandomize();
		void imguiSphlIncreaseDegrees();
		void imguiSphlDecreaseDegrees();
		void imguiSphlCoronaToSPH(size_t which);
		void imguiSphlSPHtoCubeMap(size_t which);
		void imguiSphlSaveToOBJ();

		void imguiToolsTakeScreenshot();
		void imguiToolsSaveStats();
		void imguiToolsResetStats();

		void imguiShowMenuBar();
		void imguiShowToolWindow();
		void imguiShowRenderConfigWindow();
		void imguiShowScenegraphEditor();
		void imguiShowSphlEditor();
		void imguiShowMaterialEditor();
		void Corona_GenerateSCN();
		void Corona_GenerateREF();
		void Corona_GenerateSphlVIZ();
		void Corona_GenerateSphlINIT();
		void Corona_GenerateSphlHIER();
		void Corona_GenerateSphlGEN();
		void Corona_CheckCache();
		void Corona_EraseCache();
		void Corona_GenerateTestProducts();
		void Corona_GenerateCompareProduct(bool ks, int mrd, int pl, int md);
		void Corona_EraseTestProducts();

		void imguiShowUfWindow();
		void imguiShowSSPHHWindow();
		void imguiShowTestWindow();

		void imguiUfBrokerControls();
		void imguiUfWorkerEchoControls();
		void imguiUfWorkerCoronaControls();
		void imguiUfClientGLES20Controls();
		void imguiUfClientGLES30Controls();

		void imguiMatrix4fEditControl(int id, Matrix4f& m);

		// PBSKY ModelViewController

	private:
		PbskyViewController* vcPbsky = nullptr;

	public:
		// TESTS

		void Test();
		void TestSPHLs();

		// RENDERING

		void RenderHUD();
		void RenderMainHUD();
		void RenderImGuiHUD();
		void RenderHelp();
		void RenderDeferredHUD();

		// TASKS
		int frameNumber() const { return int(cameraAnimationTime * 10000); }
		std::string frameName() const;
		void SaveScreenshot();

		static std::string GetPathTracerName(
			const std::string& sceneName,
			const std::string& mode,
			bool ks,
			int mrd,
			int pl);
		static std::string GetSphlRenderName(const std::string& sceneName, const std::string& mode, int md);
		static std::string GetPathTracerSphlRenderName(
			const std::string& sceneName,
			const std::string& mode,
			bool ks,
			int mrd,
			int pl,
			int md);
		static std::string GetStatsName(
			const std::string& sceneName,
			const std::string& mode,
			bool ks,
			int mrd,
			int pl,
			int md);

		void DirtySPHLs();
		void UpdateSPHLs();
		void UploadSPHLs();

		// ssphhapp_render*.cpp
		const int MAX_RENDER_MODES = 3;
		int renderMode = 2;
		void SetupRenderGLES30();

		// RendererConfig defaultRenderConfig;
		// RendererConfig gbufferRenderConfig;
		// RendererConfig cubeShadowRenderConfig;
		// RendererConfig cubeEnvMapRenderConfig;
		// RendererConfig rectShadowRenderConfig;
		// RendererConfig rectEnvMapRenderConfig;
		// RendererConfig uberShaderRenderConfig;
		// RendererConfig foursplitULRenderConfig;
		// RendererConfig foursplitURRenderConfig;
		// RendererConfig foursplitLLRenderConfig;
		// RendererConfig foursplitLRRenderConfig;

		// RendererSampler defaultCubeTextureSampler;
		// RendererSampler default2DTextureSampler;
		// RendererSampler defaultShadowCubeTextureSampler;
		// RendererSampler defaultShadow2DTextureSampler;

		// SimpleTexture enviroCubeTexture1;
		// SimpleTexture enviroCubeTexture2;
		// RendererTextureObject enviroSkyBoxTexture;

		// TMatrix4<GLfloat> screenPerspMatrix;
		TMatrix4<GLfloat> screenOrthoMatrix;

		RendererContextPtr rendererContext;

		void RenderFixedFunctionGL();
		void RenderGLES20();
		void RenderGLES30();
		// void RenderGLES30Scene();
		void RenderGLES30Shadows();
		void RenderGLES30SPHLs();
		void RenderGL11Hierarchies();
		void RenderGLES30_SkyBox();
		void RenderGLES30_SceneGraph();
		void RenderGLES30_VIZ();
		void RenderGLES30_PostProcess();

		void RenderTest1SunShadows();
		void RenderTest2SphereCubeMap();
		void RenderTest3EnviroCubeMap();

		// ssphhapp.cpp
		void ParseCommandArguments(const std::vector<std::string>& args);

		// Unicornfish Methods
		void InitUnicornfish();
		void KillUnicornfish();

		void GI_ScatterJobs();
		int GI_GatherJobs();
		bool GI_ProcessJob(Uf::CoronaJob& job);
		bool GI_ProcessGatherJob(Uf::CoronaJob& job);

		// SPHL METHODS
		SimpleGeometryMesh ssphh_hierarchy_mesh;
		Matrix4f I;
		FxModel geosphere;
		// SphlViz sphl;
		static constexpr int MaxShLights = 16;
		std::map<size_t, SphlViz> sphls;
		bool coefs_init = false;
		// End SPHL code

	public:
		HUDInfo my_hud_info;
		std::map<std::string, Vf::WidgetPtr> hotkeyWindows;
		// Df::PythonInterpreter python;

		std::vector<std::string> cmd_args; // to pull in the argc, argv from main()

		using SharedPtr = std::shared_ptr<SSPHH_Application>;
		using UniquePtr = std::unique_ptr<SSPHH_Application>;

		template <class... _Types>
		static SharedPtr MakeShared(_Types&&... _Args) {
			return SharedPtr(new SSPHH_Application(std::forward<_Types>(_Args)...));
		}
		template <class... _Types>
		static UniquePtr MakeUnique(_Types&&... _Args) {
			return UniquePtr(new SSPHH_Application(std::forward<_Types>(_Args)...));
		}

		SSPHH_Application();
		SSPHH_Application(const std::string& name);
		~SSPHH_Application();

		void OnInit(const std::vector<std::string>& args) override;
		void OnKill() override;

		virtual void OnUpdate(double timeStamp) override;
		virtual void DoInterfaceUpdate(double deltaTime);
		const Matrix4f& GetCameraMatrix() const;

		const std::string& GetSceneName() const { return Interface->sceneName; }

		CameraAnimation cameraAnimation;
		float cameraAnimationSpeed{ 1.0f };
		float cameraAnimationTime{ 0.0f };
		int cameraAnimationKeyframe{ 0 };
		void PathAnim_LoadCameraPath(const std::string& path);
		void PathAnim_SaveCameraPath(const std::string& path);
		void PathAnim_Play();
		void PathAnim_Stop();
		void PathAnim_Toggle();
		void PathAnim_Update();

		std::unique_ptr<InterfaceInfo> Interface = std::make_unique<InterfaceInfo>();
		int counter = 0;
		double framesPerSecond = 0.0;

		// void OnKeyDown(int key);
		// void OnKeyUp(int key);
		// void OnSpecialKeyDown(int key);
		// void OnSpecialKeyUp(int key);

		void OnKeyDown(const std::string& key, int modifiers) override;
		void OnKeyUp(const std::string& key, int modifiers) override;

		// void OnMouseMove(int X, int y, int dx, int dy);
		// void OnMouseButtonDown(int X, int y, int button);
		// void OnMouseButtonUp(int X, int y, int button);

		void OnMouseMove(int x, int y) override;
		void OnMouseButtonDown(int button) override;
		void OnMouseButtonUp(int button) override;
		void OnMouseClick(int button, const MouseClickState& mcs) override;
		void OnMouseDoubleClick(int button, const MouseDoubleClickState& mdcs) override;
		void OnMouseDrag(int button, const MouseDragState& mds) override;

		void OnPreRender() override;
		void OnRender3D() override;
		void OnRender2D() override;
		void OnRenderDearImGui() override;
		void OnPostRender() override;
		void OnReshape(int width, int height) override;

		void Sun_AdvanceClock(double numSeconds, bool recomputeSky = false);
		void Sun_ResetClock();
		void Sun_UseCurrentTime();
		void Sun_SetLights();
		void InitRenderConfigs();
		void LoadRenderConfigs();

		void RunJob(Uf::CoronaJob& job);

	private:
		void ReloadRenderConfigs();
		void StartPython();
	};
} // namespace SSPHH

using SSPHHPtr = std::shared_ptr<SSPHH::SSPHH_Application>;

// extern SSPHH_Application ssphh;
extern SSPHHPtr ssphh_widget_ptr;
void InitSSPHH();
void KillSSPHH();

#endif
