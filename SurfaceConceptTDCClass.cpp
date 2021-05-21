/*
 * The MIT License
 *
 * Copyright 2016-2018 Surface Concept GmbH
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
static const char *RcsId      = "$Id:  $";
static const char *TagName    = "$Name:  $";
static const char *CvsPath    = "$Source:  $";
static const char *SvnPath    = "$HeadURL:  $";
static const char *HttpServer = "http://www.esrf.eu/computing/cs/tango/tango_doc/ds_doc/";

#include <SurfaceConceptTDCClass.h>

//-------------------------------------------------------------------
/**
 *	Create SurfaceConceptTDCClass singleton and
 *	return it in a C function for Python usage
 */
//-------------------------------------------------------------------
extern "C" {
#ifdef _TG_WINDOWS_

__declspec(dllexport)

#endif

	Tango::DeviceClass *_create_SurfaceConceptTDC_class(const char *name) {
		return SurfaceConceptTDC_ns::SurfaceConceptTDCClass::init(name);
	}
}

namespace SurfaceConceptTDC_ns
{
//===================================================================
//	Initialize pointer for singleton pattern
//===================================================================
SurfaceConceptTDCClass *SurfaceConceptTDCClass::_instance = NULL;

//--------------------------------------------------------
/**
 * method : 		SurfaceConceptTDCClass::SurfaceConceptTDCClass(string &s)
 * description : 	constructor for the SurfaceConceptTDCClass
 *
 * @param s	The class name
 */
//--------------------------------------------------------
SurfaceConceptTDCClass::SurfaceConceptTDCClass(string &s):Tango::DeviceClass(s)
{
	cout2 << "Entering SurfaceConceptTDCClass constructor" << endl;
	set_default_property();
	write_class_property();

	/*----- PROTECTED REGION ID(SurfaceConceptTDCClass::constructor) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDCClass::constructor

	cout2 << "Leaving SurfaceConceptTDCClass constructor" << endl;
}

//--------------------------------------------------------
/**
 * method : 		SurfaceConceptTDCClass::~SurfaceConceptTDCClass()
 * description : 	destructor for the SurfaceConceptTDCClass
 */
//--------------------------------------------------------
SurfaceConceptTDCClass::~SurfaceConceptTDCClass()
{
	/*----- PROTECTED REGION ID(SurfaceConceptTDCClass::destructor) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDCClass::destructor

	_instance = NULL;
}


//--------------------------------------------------------
/**
 * method : 		SurfaceConceptTDCClass::init
 * description : 	Create the object if not already done.
 *                  Otherwise, just return a pointer to the object
 *
 * @param	name	The class name
 */
//--------------------------------------------------------
SurfaceConceptTDCClass *SurfaceConceptTDCClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			string s(name);
			_instance = new SurfaceConceptTDCClass(s);
		}
		catch (bad_alloc &)
		{
			throw;
		}		
	}		
	return _instance;
}

//--------------------------------------------------------
/**
 * method : 		SurfaceConceptTDCClass::instance
 * description : 	Check if object already created,
 *                  and return a pointer to the object
 */
//--------------------------------------------------------
SurfaceConceptTDCClass *SurfaceConceptTDCClass::instance()
{
	if (_instance == NULL)
	{
		cerr << "Class is not initialised !!" << endl;
		exit(-1);
	}
	return _instance;
}



//===================================================================
//	Command execution method calls
//===================================================================
//--------------------------------------------------------
/**
 * method : 		StartClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *StartClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "StartClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->start());
	return new CORBA::Any();
}

//--------------------------------------------------------
/**
 * method : 		StopClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *StopClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "StopClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->stop());
	return new CORBA::Any();
}

//--------------------------------------------------------
/**
 * method : 		AcquisitionStartClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *AcquisitionStartClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "AcquisitionStartClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->acquisition_start());
	return new CORBA::Any();
}

//--------------------------------------------------------
/**
 * method : 		AcquisitonStopClass::execute()
 * description : 	method to trigger the execution of the command.
 *
 * @param	device	The device on which the command must be executed
 * @param	in_any	The command input data
 *
 *	returns The command output data (packed in the Any object)
 */
//--------------------------------------------------------
CORBA::Any *AcquisitonStopClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "AcquisitonStopClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->acquisition_stop());
	return new CORBA::Any();
}


//===================================================================
//	Properties management
//===================================================================
//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDCClass::get_class_property()
 *	Description : Get the class property for specified name.
 */
//--------------------------------------------------------
Tango::DbDatum SurfaceConceptTDCClass::get_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, returns  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDCClass::get_default_device_property()
 *	Description : Return the default value for device property.
 */
//--------------------------------------------------------
Tango::DbDatum SurfaceConceptTDCClass::get_default_device_property(string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDCClass::get_default_class_property()
 *	Description : Return the default value for class property.
 */
//--------------------------------------------------------
Tango::DbDatum SurfaceConceptTDCClass::get_default_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}


//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDCClass::set_default_property()
 *	Description : Set default property (class and device) for wizard.
 *                For each property, add to wizard property name and description.
 *                If default value has been set, add it to wizard property and
 *                store it in a DbDatum.
 */
//--------------------------------------------------------
void SurfaceConceptTDCClass::set_default_property()
{
	string	prop_name;
	string	prop_desc;
	string	prop_def;
	vector<string>	vect_data;

	//	Set Default Class Properties

	//	Set Default device Properties
        // IniFilePath
	prop_name = "IniFilePath";
	prop_desc = "Path and Filename of the Surface Concept TDC .ini file";
	prop_def  = "tdc_gpx3_simulation.ini";
	vect_data.clear();
	vect_data.push_back("tdc_gpx3_simulation.ini");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
        // DefaultLiveOut
	prop_name = "DefaultLiveOut";
	prop_desc = "Default Target for live output";
	prop_def  = "/run/user/1000/tdc1";
	vect_data.clear();
	vect_data.push_back("/run/user/1000/tdc1");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
        // LivePreviewModeFileActive
	prop_name = "LivePreviewModeFileActive";
	prop_desc = "if true, use file output of images (xy, xt, yt: live and accumulation preview)";
	prop_def  = "0";
	vect_data.clear();
	vect_data.push_back("0");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);
        // LivePreviewModeTangoActive
	prop_name = "LivePreviewModeTangoActive";
	prop_desc = "if true, use Tango ImageAttributes for images (xy, xt, yt: live and accumulation preview)";
	prop_def  = "1";
	vect_data.clear();
	vect_data.push_back("1");
	if (prop_def.length()>0)
	{
		Tango::DbDatum	data(prop_name);
		data << vect_data ;
		dev_def_prop.push_back(data);
		add_wiz_dev_prop(prop_name, prop_desc,  prop_def);
	}
	else
		add_wiz_dev_prop(prop_name, prop_desc);        
}

//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDCClass::write_class_property()
 *	Description : Set class description fields as property in database
 */
//--------------------------------------------------------
void SurfaceConceptTDCClass::write_class_property()
{
	//	First time, check if database used
	if (Tango::Util::_UseDb == false)
		return;

	Tango::DbData	data;
	string	classname = get_name();
	string	header;
	string::size_type	start, end;

	//	Put title
	Tango::DbDatum	title("ProjectTitle");
	string	str_title("SurfaceConceptTDC");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	vector<string>	str_desc;
	str_desc.push_back("");
	description << str_desc;
	data.push_back(description);

	//	put cvs or svn location
	string	filename("SurfaceConceptTDC");
	filename += "Class.cpp";

	// check for cvs information
	string	src_path(CvsPath);
	start = src_path.find("/");
	if (start!=string::npos)
	{
		end   = src_path.find(filename);
		if (end>start)
		{
			string	strloc = src_path.substr(start, end-start);
			//	Check if specific repository
			start = strloc.find("/cvsroot/");
			if (start!=string::npos && start>0)
			{
				string	repository = strloc.substr(0, start);
				if (repository.find("/segfs/")!=string::npos)
					strloc = "ESRF:" + strloc.substr(start, strloc.length()-start);
			}
			Tango::DbDatum	cvs_loc("cvs_location");
			cvs_loc << strloc;
			data.push_back(cvs_loc);
		}
	}

	// check for svn information
	else
	{
		string	src_path(SvnPath);
		start = src_path.find("://");
		if (start!=string::npos)
		{
			end = src_path.find(filename);
			if (end>start)
			{
				header = "$HeadURL: ";
				start = header.length();
				string	strloc = src_path.substr(start, (end-start));
				
				Tango::DbDatum	svn_loc("svn_location");
				svn_loc << strloc;
				data.push_back(svn_loc);
			}
		}
	}

	//	Get CVS or SVN revision tag
	
	// CVS tag
	string	tagname(TagName);
	header = "$Name: ";
	start = header.length();
	string	endstr(" $");
	
	end   = tagname.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strtag = tagname.substr(start, end-start);
		Tango::DbDatum	cvs_tag("cvs_tag");
		cvs_tag << strtag;
		data.push_back(cvs_tag);
	}
	
	// SVN tag
	string	svnpath(SvnPath);
	header = "$HeadURL: ";
	start = header.length();
	
	end   = svnpath.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strloc = svnpath.substr(start, end-start);
		
		string tagstr ("/tags/");
		start = strloc.find(tagstr);
		if ( start!=string::npos )
		{
			start = start + tagstr.length();
			end   = strloc.find(filename);
			string	strtag = strloc.substr(start, end-start-1);
			
			Tango::DbDatum	svn_tag("svn_tag");
			svn_tag << strtag;
			data.push_back(svn_tag);
		}
	}

	//	Get URL location
	string	httpServ(HttpServer);
	if (httpServ.length()>0)
	{
		Tango::DbDatum	db_doc_url("doc_url");
		db_doc_url << httpServ;
		data.push_back(db_doc_url);
	}

	//  Put inheritance
	Tango::DbDatum	inher_datum("InheritedFrom");
	vector<string> inheritance;
	inheritance.push_back("TANGO_BASE_CLASS");
	inher_datum << inheritance;
	data.push_back(inher_datum);

	//	Call database and and values
	get_db_class()->put_property(data);
}

//===================================================================
//	Factory methods
//===================================================================

//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDCClass::device_factory()
 *	Description : Create the device object(s)
 *                and store them in the device list
 */
//--------------------------------------------------------
void SurfaceConceptTDCClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
	/*----- PROTECTED REGION ID(SurfaceConceptTDCClass::device_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDCClass::device_factory_before

	//	Create devices and add it into the device list
	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i].in() << endl;
		device_list.push_back(new SurfaceConceptTDC(this, (*devlist_ptr)[i]));							 
	}

	//	Manage dynamic attributes if any
	erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());

	//	Export devices to the outside world
	for (unsigned long i=1 ; i<=devlist_ptr->length() ; i++)
	{
		//	Add dynamic attributes if any
		SurfaceConceptTDC *dev = static_cast<SurfaceConceptTDC *>(device_list[device_list.size()-i]);
		dev->add_dynamic_attributes();

		//	Check before if database used.
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(dev);
		else
			export_device(dev, dev->get_name().c_str());
	}

	/*----- PROTECTED REGION ID(SurfaceConceptTDCClass::device_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDCClass::device_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDCClass::attribute_factory()
 *	Description : Create the attribute object(s)
 *                and store them in the attribute list
 */
//--------------------------------------------------------
void SurfaceConceptTDCClass::attribute_factory(vector<Tango::Attr *> &att_list)
{
	/*----- PROTECTED REGION ID(SurfaceConceptTDCClass::attribute_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDCClass::attribute_factory_before
	//	Attribute : DeviceID
	DeviceIDAttrib	*deviceid = new DeviceIDAttrib();
	Tango::UserDefaultAttrProp	deviceid_prop;
	//	description	not set for DeviceID
	//	label	not set for DeviceID
	//	unit	not set for DeviceID
	//	standard_unit	not set for DeviceID
	//	display_unit	not set for DeviceID
	//	format	not set for DeviceID
	//	max_value	not set for DeviceID
	//	min_value	not set for DeviceID
	//	max_alarm	not set for DeviceID
	//	min_alarm	not set for DeviceID
	//	max_warning	not set for DeviceID
	//	min_warning	not set for DeviceID
	//	delta_t	not set for DeviceID
	//	delta_val	not set for DeviceID
	
	deviceid->set_default_properties(deviceid_prop);
	//	Not Polled
	deviceid->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	att_list.push_back(deviceid);

	//	Attribute : ExposureLive
	ExposureLiveAttrib	*exposurelive = new ExposureLiveAttrib();
	Tango::UserDefaultAttrProp	exposurelive_prop;
        exposurelive_prop.description = "Exposure Time for Live Images";
        exposurelive_prop.unit = "s";
        exposurelive_prop.standard_unit = "s";
        exposurelive_prop.display_unit = "s";
        exposurelive_prop.format = "%4.1f";
        exposurelive_prop.min_value = "0.0";
	//	label	not set for ExposureLive
	//	max_value	not set for ExposureLive
	//	max_alarm	not set for ExposureLive
	//	min_alarm	not set for ExposureLive
	//	max_warning	not set for ExposureLive
	//	min_warning	not set for ExposureLive
	//	delta_t	not set for ExposureLive
	//	delta_val	not set for ExposureLive
	
	exposurelive->set_default_properties(exposurelive_prop);
	//	Not Polled
	exposurelive->set_disp_level(Tango::OPERATOR);
	exposurelive->set_memorized();
	exposurelive->set_memorized_init(true);
	att_list.push_back(exposurelive);

	//	Attribute : ExposureAccu
	ExposureAccuAttrib	*exposureaccu = new ExposureAccuAttrib();
	Tango::UserDefaultAttrProp	exposureaccu_prop;
        exposureaccu_prop.description = "Exposure Time for Accumulation of Images";
        exposureaccu_prop.unit = "s";
        exposureaccu_prop.standard_unit = "s";
        exposureaccu_prop.display_unit = "s";
        exposureaccu_prop.format = "%5.1f";
        exposureaccu_prop.min_value = "0.0";
	//	label	not set for ExposureAccu
	//	max_value	not set for ExposureAccu
	//	max_alarm	not set for ExposureAccu
	//	min_alarm	not set for ExposureAccu
	//	max_warning	not set for ExposureAccu
	//	min_warning	not set for ExposureAccu
	//	delta_t	not set for ExposureAccu
	//	delta_val	not set for ExposureAccu
	
	exposureaccu->set_default_properties(exposureaccu_prop);
	//	Not Polled
	exposureaccu->set_disp_level(Tango::OPERATOR);
	exposureaccu->set_memorized();
	exposureaccu->set_memorized_init(true);
	att_list.push_back(exposureaccu);

	//	Create a list of static attributes
	create_static_attribute_list(get_class_attr()->get_attr_list());
	/*----- PROTECTED REGION ID(SurfaceConceptTDCClass::attribute_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDCClass::attribute_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDCClass::command_factory()
 *	Description : Create the command object(s)
 *                and store them in the command list
 */
//--------------------------------------------------------
void SurfaceConceptTDCClass::command_factory()
{
	/*----- PROTECTED REGION ID(SurfaceConceptTDCClass::command_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDCClass::command_factory_before


	//	Command Start
	StartClass	*pStartCmd =
		new StartClass("DeviceStart",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pStartCmd);

	//	Command Stop
	StopClass	*pStopCmd =
		new StopClass("DeviceStop",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pStopCmd);

	//	Command AcquisitionStart
	AcquisitionStartClass	*pAcquisitionStartCmd =
		new AcquisitionStartClass("AcquisitionStart",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pAcquisitionStartCmd);

	//	Command AcquisitonStop
	AcquisitonStopClass	*pAcquisitonStopCmd =
		new AcquisitonStopClass("AcquisitionStop",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pAcquisitonStopCmd);

	/*----- PROTECTED REGION ID(SurfaceConceptTDCClass::command_factory_after) ENABLED START -----*/
	//	Add your own code
	//	Command SaveXytToTiff
	SaveXytToTiffClass	*pSaveXytToTiffCmd =
		new SaveXytToTiffClass("SaveXytToTiff",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pSaveXytToTiffCmd);	

	//	Command SaveXyToTiff
	SaveXyToTiffClass	*pSaveXyToTiffCmd =
		new SaveXyToTiffClass("SaveXyToTiff",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pSaveXyToTiffCmd);
        
	//	Command SaveXyToText
	SaveXyToTextClass	*pSaveXyToTextCmd =
		new SaveXyToTextClass("SaveXyToText",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pSaveXyToTextCmd);        

	//	Command AccumStartAndSaveXY
	AccumStartAndSaveXYClass    *pAccumStartAndSaveXYCmd =
		new AccumStartAndSaveXYClass("AccumStartAndSaveXY",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pAccumStartAndSaveXYCmd);
        
	//	Command AccumStartAndSaveXYText
	AccumStartAndSaveXYTextClass    *pAccumStartAndSaveXYTextCmd =
		new AccumStartAndSaveXYTextClass("AccumStartAndSaveXYText",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pAccumStartAndSaveXYTextCmd);        

	//	Command AccumulationStart
	AccumulationStartClass	*pAccumulationStartCmd =
		new AccumulationStartClass("AccumulationStart",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pAccumulationStartCmd);
	
	//	Command AccumulationContinue
	AccumulationContinueClass	*pAccumulationContinueCmd =
		new AccumulationContinueClass("AccumulationContinue",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pAccumulationContinueCmd);
        
	//	Command AccumulationStop
	AccumulationStopClass	*pAccumulationStopCmd =
		new AccumulationStopClass("AccumulationStop",
			Tango::DEV_VOID, Tango::DEV_VOID,
			"",
			"",
			Tango::OPERATOR);
	command_list.push_back(pAccumulationStopCmd);

	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDCClass::command_factory_after
}

//===================================================================
//	Dynamic attributes related methods
//===================================================================

//--------------------------------------------------------
/**
 * method : 		SurfaceConceptTDCClass::create_static_attribute_list
 * description : 	Create the a list of static attributes
 *
 * @param	att_list	the ceated attribute list 
 */
//--------------------------------------------------------
void SurfaceConceptTDCClass::create_static_attribute_list(vector<Tango::Attr *> &att_list)
{
	for (unsigned long i=0 ; i<att_list.size() ; i++)
	{
		string att_name(att_list[i]->get_name());
		transform(att_name.begin(), att_name.end(), att_name.begin(), ::tolower);
		defaultAttList.push_back(att_name);
	}

	cout2 << defaultAttList.size() << " attributes in default list" << endl;

	/*----- PROTECTED REGION ID(SurfaceConceptTDCClass::create_static_att_list) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDCClass::create_static_att_list
}


//--------------------------------------------------------
/**
 * method : 		SurfaceConceptTDCClass::erase_dynamic_attributes
 * description : 	delete the dynamic attributes if any.
 *
 * @param	devlist_ptr	the device list pointer
 * @param	list of all attributes
 */
//--------------------------------------------------------
void SurfaceConceptTDCClass::erase_dynamic_attributes(const Tango::DevVarStringArray *devlist_ptr, vector<Tango::Attr *> &att_list)
{
	Tango::Util *tg = Tango::Util::instance();

	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{	
		Tango::DeviceImpl *dev_impl = tg->get_device_by_name(((string)(*devlist_ptr)[i]).c_str());
		SurfaceConceptTDC *dev = static_cast<SurfaceConceptTDC *> (dev_impl);
		
		vector<Tango::Attribute *> &dev_att_list = dev->get_device_attr()->get_attribute_list();
		vector<Tango::Attribute *>::iterator ite_att;
		for (ite_att=dev_att_list.begin() ; ite_att != dev_att_list.end() ; ++ite_att)
		{
			string att_name((*ite_att)->get_name_lower());
			if ((att_name == "state") || (att_name == "status"))
				continue;
			vector<string>::iterator ite_str = find(defaultAttList.begin(), defaultAttList.end(), att_name);
			if (ite_str == defaultAttList.end())
			{
				cout2 << att_name << " is a UNWANTED dynamic attribute for device " << (*devlist_ptr)[i] << endl;
				Tango::Attribute &att = dev->get_device_attr()->get_attr_by_name(att_name.c_str());
				dev->remove_attribute(att_list[att.get_attr_idx()], true, false);
				--ite_att;
			}
		}
	}
	/*----- PROTECTED REGION ID(SurfaceConceptTDCClass::erase_dynamic_attributes) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDCClass::erase_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method      : SurfaceConceptTDCClass::get_attr_by_name()
 *	Description : returns Tango::Attr * object found by name
 */
//--------------------------------------------------------
Tango::Attr *SurfaceConceptTDCClass::get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname)
{
	vector<Tango::Attr *>::iterator it;
	for (it=att_list.begin() ; it<att_list.end() ; it++)
		if ((*it)->get_name()==attname)
			return (*it);
	//	Attr does not exist
	return NULL;
}


/*----- PROTECTED REGION ID(SurfaceConceptTDCClass::Additional Methods) ENABLED START -----*/

CORBA::Any *SaveXytToTiffClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "SaveXytToTiffClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->save_xyt_to_tiff());
	return new CORBA::Any();
}

CORBA::Any *SaveXyToTiffClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "SaveXyToTiffClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->save_xy_to_tiff());
	return new CORBA::Any();
}

CORBA::Any *SaveXyToTextClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "SaveXyToTextClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->save_xy_to_text());
	return new CORBA::Any();
}

CORBA::Any *AccumulationStartClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "AccumulationStartClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->accumulation_start());
	return new CORBA::Any();
}

CORBA::Any *AccumulationContinueClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "AccumulationContinueClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->accumulation_continue());
	return new CORBA::Any();
}

CORBA::Any *AccumulationStopClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "AccumulationStopClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->accumulation_stop());
	return new CORBA::Any();
}

CORBA::Any *AccumStartAndSaveXYClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "AccumStartAndSaveXYClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->accum_start_and_save_xy());
	return new CORBA::Any();
}

CORBA::Any *AccumStartAndSaveXYTextClass::execute(Tango::DeviceImpl *device, TANGO_UNUSED(const CORBA::Any &in_any))
{
	cout2 << "AccumStartAndSaveXYTextClass::execute(): arrived" << endl;
	((static_cast<SurfaceConceptTDC *>(device))->accum_start_and_save_xy_text());
	return new CORBA::Any();
}



/*----- PROTECTED REGION END -----*/	//	SurfaceConceptTDCClass::Additional Methods
} //	namespace
