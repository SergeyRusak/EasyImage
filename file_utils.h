#pragma once


#include "ruclip/TorchHeader.h"

#include <cmath>
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "ruclip/RuCLIP.h"
#include "ruclip/RuCLIPProcessor.h"

#include <iostream>
#include <filesystem>
#include<iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <wx/log.h>
#include "include/BS_thread_pool.hpp"


class file_utils
{
public:
	std::vector<std::filesystem::path> find_images(std::vector<std::filesystem::path> start_dirs, bool is_recursive = true)
	{
		std::vector<std::filesystem::path> dirs = start_dirs;
		std::vector<std::filesystem::path> files;

		BS::thread_pool pool;
		std::list<std::future<std::vector<std::filesystem::path>>> multy_future;
		while (multy_future.size() > 0 || dirs.size() > 0) {
			while (dirs.size() > 0) {
				std::filesystem::path path = dirs.front();
				dirs.erase(dirs.begin());
				multy_future.push_back(pool.submit_task([path] {return task(path); }));
			}
			std::list<std::future<std::vector<std::filesystem::path>>>::iterator i = multy_future.begin();
			std::list<std::future<std::vector<std::filesystem::path>>>::iterator end = multy_future.end();
			while (i != end) {
				if ((*i).wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
					std::vector<std::filesystem::path> fres = (*i).get();
					while (fres.front().string() != "separate") {
						if (is_recursive) dirs.push_back(fres.front());
						fres.erase(fres.begin());
					}
					fres.erase(fres.begin());
					files.insert(files.end(), fres.begin(), fres.end());
					i = multy_future.erase(i);
				} else ++i;
			}
		};
		return files;
	}

	void loadmodel() {
		torch::manual_seed(24);

		torch::Device device(torch::kCPU);
		if (torch::cuda::is_available())
		{
			std::cout << "CUDA is available! Running on GPU." << std::endl;
			device = torch::Device(torch::kCUDA);
		}
		else {
			std::cout << "CUDA is not available! Running on CPU." << std::endl;
		}

		std::cout << "Load clip from: " << pathToClip << std::endl;
		CLIP clip = FromPretrained(pathToClip);
		clip->to(device);

		std::cout << "Load processor from: " << pathToBPE << std::endl;
		RuCLIPProcessor processor(
			pathToBPE,
			INPUT_IMG_SIZE,
			77,
			{ 0.48145466, 0.4578275, 0.40821073 },
			{ 0.26862954, 0.26130258, 0.27577711 }
		);
		int INPUT_IMG_SIZE = parser.get<int>("img_size");
	}
	torch::Tensor image_analize(wxArrayString img_paths)
	{
				
		std::vector<cv::Mat> images;
		{
			for (size_t i = 0; i < img_paths.size(); i++)
			{
				cv::Mat img = cv::imread(cv::String(img_paths.Item(i)), cv::IMREAD_COLOR);

				cv::resize(img, img, cv::Size(INPUT_IMG_SIZE, INPUT_IMG_SIZE), cv::INTER_CUBIC);
				images.emplace_back(img);
			}
		}
		std::cout << "Running..." << std::endl;
		auto dummy_input = processor(labels, images);
		try {
			torch::Tensor logits_per_image = clip->forward(dummy_input.first.to(device), dummy_input.second.to(device));
			auto probs = logits_per_image.softmax(/*dim = */-1).detach().cpu();
			std::cout << "probs per image: " << probs << std::endl;
			return probs;
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		std::cout << "The end!" << std::endl;
	}



	}



torch::Tensor text_analize(){
		//             
		std::vector<std::string> labels;
		{
			std::cout << "labels: ";
			std::regex sep("[,]+");
			std::sregex_token_iterator tokens(labelsStr.cbegin(), labelsStr.cend(), sep, -1);
			std::sregex_token_iterator end;
			for (; tokens != end; ++tokens)
			{
				std::cout << (*tokens) << " | ";
				labels.emplace_back(*tokens);
			}
			std::cout << std::endl;
		}

		std::cout << "Running..." << std::endl;
		auto dummy_input = processor(labels, images);
		try {
			torch::Tensor logits_per_text = clip->forward(dummy_input.first.to(device), dummy_input.second.to(device));
			auto probs = logits_per_text.softmax(/*dim = */-1).detach().cpu();
			std::cout << "probs per text: " << probs << std::endl;
			return probs;
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		
	}



private:
	static std::vector<std::filesystem::path> task(std::filesystem::path dir_path) {
		std::vector<std::filesystem::path> paths;
		paths.push_back("separate");
		for (const auto& entry : std::filesystem::directory_iterator(dir_path))
			try {
			if (entry.is_regular_file()) {
				std::string exten = entry.path().extension().string();
				std::string imgtypes = ".001,.1sc,.2bp,.360,.3d,.3d2,.3d4,.3da,.3dc,.3df,.3dl,.3dm,.3dmf,.3dmk,.3don,.3dp,.3ds,.3dv,.3dw,.3dx,.3dxml,.3fr,.3mf,.411,.73i,.8ca,.8ci,.8pbs,.8xi,.9.png,.@ex,.a2c,.a3d,.a8s,.abc,.abm,.ac5,.ac6,.acorn,.acr,.act,.adc,.af2,.af3,.afdesign,.afphoto,.afx,.agif,.agp,.ai,.aic,.ais,.ait,.albm,.album,.amdn,.amf,.an8,.anim,.animset,.anm,.aof,.aoi,.apd,.apm,.apng,.aps,.apx,.arexport,.arfx,.ari,.arm,.arpatch,.arproj,.arprojpkg,.arr,.art,.artb,.artwork,.arw,.asat,.ase,.aseprite,.asy,.atf,.atl,.atm,.avatar,.avb,.avif,.avifs,.awd,.awg,.b3d,.bay,.bbmodel,.bif,.bio,.bip,.bld,.blend,.blk,.blkrt,.blz,.bm2,.bmc,.bmf,.bmp,.bmq,.bmx,.bmz,.bpg,.br3,.br4,.br5,.br6,.br7,.brg,.brk,.brn,.bro,.brt,.brush,.bs,.bsk,.bss,.bti,.bto,.bvh,.bw,.c3d,.c3z,.c4,.c4d,.caf,.cag,.cal,.cals,.cam,.can,.cas,.cbddlp,.ccb,.cci,.ccp,.ccx,.cd5,.cdc,.cdd,.cddz,.cdg,.cdlx,.cdmm,.cdmt,.cdmtz,.cdmz,.cdr,.cdrapp,.cds,.cdsx,.cdt,.cdtx,.cdx,.ce,.cfg,.cg,.cg3,.cga,.cgfx,.cgm,.chr,.chrparams,.cid,.cil,.cimg,.cin,.cit,.clara,.clarify,.clip,.cm2,.cmdb,.cmf,.cmod,.cmr,.cms,.cmx,.cmz,.cnv,.colz,.cor,.cpbitmap,.cpc,.cpd,.cpg,.cps,.cpt,.cpx,.cpy,.cr2,.cr3,.crf,.crw,.crz,.cs1,.csd,.csf,.csm,.cso,.csy,.ct,.cut,.cv5,.cvd,.cvdtpl,.cvg,.cvi,.cvs,.cvx,.cvxcad,.cwt,.cxi,.d3d,.dae,.daz,.dbc,.dbl,.dbm,.dbs,.dc2,.dc6,.dcm,.dcr,.dcs,.dcx,.ddb,.ddd,.ddp,.ddrw,.dds,.ddt,.ded,.des,.design,.dff,.dfs,.dgt,.dhs,.dia,.dib,.dic,.dicom,.dif,.djv,.djvu,.dm3,.dm4,.dmc,.dmi,.dn,.dng,.dpp,.dpr,.dpx,.drawing,.drawio,.drawit,.drf,.drp,.drw,.drz,.ds,.ds4,.dsa,.dsb,.dsd,.dse,.dsf,.dsg,.dsi,.dso,.dsv,.dt2,.dtw,.duf,.dvl,.dwf,.dxb,.e57,.ecw,.egc,.egg,.eip,.emcam,.emf,.emz,.ep,.epgz,.epp,.eps,.epsf,.erf,.esc,.exp,.exr,.ezdraw,.f3d,.fac,.face,.facefx,.fal,.fax,.fbm,.fbx,.fc2,.fcm,.fcp,.fcz,.ff,.fff,.fg,.fh10,.fh11,.fh3,.fh4,.fh5,.fh6,.fh7,.fh8,.fh9,.fhd,.fif,.fig,.fil,.fits,.flif,.flt,.fmv,.fnc,.fp,.fp3,.fpe,.fpf,.fpg,.fpj,.fpos,.fppx,.fpx,.frm,.fry,.fs,.fsh,.fsq,.fsthumb,.ft10,.ft11,.ft7,.ft8,.ft9,.ftn,.fun,.fuse,.fx,.fxa,.fxg,.fxl,.fxm,.fxs,.fxt,.g3,.g3f,.g3n,.gbr,.gcdp,.gem,.geo,.gfb,.gfie,.ggr,.gh,.ghx,.gif,.gih,.gim,.gks,.glb,.glf,.glm,.glox,.gls,.glsl,.gltf,.gmbck,.gmf,.gmmod,.gmspr,.gmt,.gp4,.gpd,.gpr,.graffle,.grn,.gro,.grob,.grs,.gry,.gsd,.gstencil,.gtemplate,.gvdesign,.gvrs,.hd2,.hdp,.hdr,.hdrp,.hdz,.heic,.heif,.hf,.hgl,.hif,.hip,.hipnc,.hlsl,.hpg,.hpgl,.hpi,.hpl,.hr,.hr2,.hrf,.hrz,.hvif,.hxn,.i3d,.iavatar,.ic1,.ic2,.ic3,.ica,.icb,.icn,.icon,.icpr,.idea,.ifc,.iges,.igi,.igm,.igs,.igt,.igx,.iiq,.ik,.ilbm,.imd,.img,.imi,.imj,.info,.ink,.insp,.int,.ipick,.ipv,.ipx,.irr,.irrmesh,.itc2,.ithmb,.iv,.ive,.ivr,.ivue,.iwi,.j,.j2c,.j2k,.j3o,.j6i,.jas,.jb2,.jbf,.jbg,.jbig,.jbig2,.jbmp,.jbr,.jcd,.jfi,.jfif,.jia,.jif,.jiff,.jls,.jng,.jp2,.jpc,.jpd,.jpe,.jpeg,.jpf,.jpg,.jpg-large,.jpg2,.jpg_large,.jpg_medium,.jpg_orig,.jpg_small,.jpg_thumb,.jps,.jpx,.jsl,.jtf,.jwl,.jxl,.jxr,.k25,.kc2,.kdc,.kdi,.kdk,.kfm,.kfx,.kic,.kmc,.kmcobj,.kodak,.kpg,.kra,.kss,.ktx,.ktx2,.ktz,.lb,.lbm,.ldm,.ldoc,.ldw,.lif,.linea,.lip,.ljp,.llm,.lmk,.lmnr,.lnd,.lp,.lps,.lrg,.lrpreview,.lt2,.ltz,.lwo,.lws,.lxf,.lxo,.lzp,.m3,.m3d,.m3g,.ma,.mac,.makerbot,.mat,.max,.maxc,.mb,.mbm,.mc,.mc5,.mc6,.mcs,.mcsg,.mcx-8,.mcz,.md5anim,.md5camera,.md5mesh,.mdc,.mdd,.mdg,.mdl,.mdp,.mdx,.meb,.mef,.mesh,.met,.mfw,.mgc,.mgcb,.mgf,.mgmf,.mgmt,.mgmx,.mgs,.mgtx,.mhm,.mic,.miff,.mip,.mipmaps,.mix,.mmat,.mmpp,.mng,.mnm,.mnr,.mos,.mot,.mp,.mpf,.mpj,.mpo,.mqo,.mrb,.mrml,.mrw,.mrxs,.ms3d,.msh,.msk,.msp,.mtl,.mtx,.mtz,.mu,.mud,.mvg,.mxi,.mxm,.mxs,.myl,.n2,.n3d,.nap,.ncd,.ncr,.nct,.ndb,.ndpi,.ndtx,.ndx,.nef,.neo,.nff,.nif,.nksc,.nlm,.nm,.nodes,.nol,.npsd,.nrw,.nsbta,.nwm,.nxs,.obj,.obp,.obz,.oc3,.oc4,.oc5,.oci,.oct,.odg,.odi,.oe6,.off,.ogf,.oir,.ol,.omf,.oplc,.ora,.orf,.ota,.otb,.otg,.oti,.ovp,.ovr,.ozb,.ozj,.ozt,.p,.p21,.p2z,.p3d,.p3l,.p3m,.p3r,.p4d,.p5d,.pac,.pal,.palm,.pam,.pano,.pap,.par,.part,.pat,.pbm,.pbs,.pc1,.pc2,.pc3,.pcd,.pcs,.pct,.pcx,.pd,.pdd,.pdn,.pe4,.pef,.pen,.pfd,.pfi,.pfr,.pfv,.pgal,.pgf,.pgm,.phy,.pi1,.pi2,.pi3,.pi4,.pi5,.pi6,.pic,.picnc,.pict,.pigm,.pigs,.piskel,.pix,.pixadex,.pixela,.pixil,.pjp,.pjpeg,.pjpg,.pkg,.pl,.pl0,.pl1,.pl2,.plt,.ply,.pm,.pm3,.pmd,.pmg,.pmx,.pnc,.png,.pni,.pnm,.pns,.pnt,.pntg,.pobj,.pop,.pov,.pp2,.pp4,.pp5,.ppf,.ppm,.ppp,.ppz,.prc,.prefab,.previz,.primitives,.prm,.pro,.procreate,.prv,.prw,.ps,.psa,.psb,.psd,.psdb,.psdc,.psdx,.pse,.psf,.psid,.psk,.psp,.pspbrush,.pspimage,.psxprj,.ptex,.ptg,.ptk,.pts,.ptx,.puppet,.pvr,.pwp,.pws,.px,.pxd,.pxicon,.pxm,.pxr,.pxz,.pyxel,.pz2,.pz3,.pza,.pzp,.pzs,.pzz,.qc,.qcc,.qif,.qmg,.qoi,.qti,.qtif,.r3d,.rad,.raf,.ras,.raw,.ray,.rcl,.rcs,.rcu,.rdl,.rds,.real,.reality,.rft,.rgb,.rgba,.rgf,.ric,.rif,.riff,.rig,.rix,.rle,.rli,.rpf,.rri,.rs,.rsb,.rsr,.rtl,.rvg,.rw2,.rwl,.rwz,.s,.s2mv,.s3g,.sai,.sai2,.sar,.sbfres,.sbp,.sbsar,.sc4model,.scg,.sci,.scn,.scp,.sct,.scu,.scv,.sda,.sdb,.sdr,.sep,.session,.sfc,.sff,.sfw,.sgd,.sgi,.sgn,.sh3d,.sh3f,.shapes,.shg,.shp,.si,.sid,.sig,.sim,.sis,.six,.sk,.sk1,.sk2,.sketch,.sketchpad,.skitch,.skl,.skm,.skp,.sktz,.sld,.slddrt,.sm,.smc,.smd,.smf,.smp,.snag,.snagproj,.snagstyles,.snagx,.snip,.sob,.spa,.spc,.spe,.sph,.spiff,.spj,.spp,.spr,.sprite,.sprite2,.sprite3,.spu,.spv,.sr,.sr2,.srf,.srw,.ssk,.stc,.std,.ste,.stel,.stex,.stn,.sto,.sumo,.sun,.suniff,.sup,.sva,.svf,.svg,.svgz,.svm,.svs,.svslide,.sxd,.t2b,.t2k,.t3d,.taac,.targa,.tb0,.tbn,.tcn,.tddd,.tex,.tex.emz,.texture,.tfc,.tg4,.tga,.tgo,.thing,.thl,.thm,.thumb,.tif,.tiff,.tilt,.tjp,.tlc,.tm2,.tmd,.tme,.tmo,.tn,.tn1,.tn2,.tn3,.tne,.tny,.tpf,.tpi,.tpl,.tps,.trace,.tri,.trif,.ts1,.tsr,.tub,.tvm,.u,.u3d,.ufo,.ufr,.uga,.ugoira,.ums,.upf,.urt,.usd,.usdz,.v,.v3d,.v3o,.v3v,.vac,.vbr,.vda,.vdoc,.vec,.veg,.vff,.vic,.vicar,.viff,.visual,.vmd,.vml,.vmo,.vmu,.vna,.vob,.vox,.vp,.vpd,.vpe,.vrimg,.vrl,.vrm,.vrphoto,.vs,.vsd,.vsdm,.vsdx,.vsh,.vso,.vss,.vst,.vstm,.vstx,.vtx,.vue,.vvd,.w3d,.wb0,.wb1,.wb2,.wbc,.wbd,.wbm,.wbmp,.wbp,.wbz,.wdp,.webp,.wft,.wi,.wic,.wlm,.wmf,.wmp,.wmz,.wow,.wp2,.wpb,.wpe,.wpg,.wpi,.wrl,.wrp,.wrz,.wvl,.x,.x3d,.x3f,.x3g,.xaf,.xar,.xbm,.xcf,.xface,.xmf,.xmm,.xmmap,.xmmat,.xof,.xpm,.xpr,.xr,.xrf,.xsf,.xsi,.xv0,.xwd,.y,.yal,.yaodl,.ydl,.ydr,.ylc,.ysp,.yuv,.z3d,.zgm,.zif,.zt,.zvi";
				std::stringstream ss (imgtypes);
				bool is_img = false;
				for (std::string t; std::getline(ss,t,',');)
					if (exten.compare(t) == 0) {
						is_img = true;
						break;
					}
				if (is_img) {
					paths.push_back(entry.path());
				}
			}
			else
				if (entry.is_directory()) {
					paths.insert(paths.begin(),entry.path());
				}
		}
		catch (std::filesystem::filesystem_error e) {
			wxLogError(e.what());
		}
		return paths;
	}
};

