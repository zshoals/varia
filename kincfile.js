const fs = require('fs');
const path = require("path");

function gen_push_line(append) {
	return append + "\n";
}

function getFileType(DirEnt_like)
{
	return DirEnt_like.name.split(".")[1];
}

function get_all_subdirectories(path_inc)
{
	let files = fs.readdirSync(path_inc, {withFileTypes: true});
	let directories = files.filter(file => file.isDirectory());
	let all_directories = [];
	if (directories.length > 0)
	{
		for (const directory of directories)
		{	
			all_directories.push(path_inc + directory.name + "/");
			all_directories = all_directories.concat(get_all_subdirectories(path_inc + directory.name + "/"));
		}
	}
	return all_directories;
}

//We'll use this format to write out all of our assets to file
//Same thing for sound, music, etc. 
//We'll need to expand this to handle recursive directories in images and sounds and such, maybe

//Ok, it works, but we need to make it work for any resource type
//Gotta remove the hardcoding and stuff


//Yikes.
//What a terrible fate.
function generate_resource_files(
	base_search_path,
	asset_type,
	arr_of_filetypes, 
	enum_prefix, 
	enum_name,
	file_gen_comment, 
	generated_filename
)
{
	
	if (fs.existsSync(base_search_path))
	{
		const basedir_path = [base_search_path];
		//First, acquire the pathnames to every single asset
		let subdir_paths = get_all_subdirectories(base_search_path);
		let all_paths = basedir_path.concat(subdir_paths); 

		let image_enums = [];
		let image_enum_names = [];
		let image_paths = [];

		for (const a_path of all_paths)
		{
			let directory_contents = fs.readdirSync(a_path, {withFileTypes: true});

			for (const file of directory_contents)
			{
				if (!file.isFile()) continue;

				let filetype = path.extname(file.name);
				if (arr_of_filetypes.includes(filetype))
				{	
					const base_file_path = a_path;
					const asset_string_path = "\"" + base_file_path.split("Sources/")[1] + file.name + "\"";
					const resource_relative_path = base_file_path.split("Sources/assets/images/")[1] + file.name;
					const enum_postfix = resource_relative_path.replace(/\//g, "_").toUpperCase().split(".")[0];

					image_paths.push(asset_string_path);
					image_enums.push(enum_prefix + enum_postfix);
					image_enum_names.push("\"" + enum_prefix + enum_postfix + "\"")
				}
			}
		}

		let autogenerated_text = "";
		let header_file = "";
		let object_file = "";

		autogenerated_text += gen_push_line("/*");
		autogenerated_text += gen_push_line("	NOTE: THIS FILE IS AUTOGENERATED VIA KMAKE");
		autogenerated_text += gen_push_line("	THIS FILE SHOULD NOT BE EDITED MANUALLY");
		autogenerated_text += gen_push_line("");
		autogenerated_text += gen_push_line("	" + file_gen_comment);
		autogenerated_text += gen_push_line("	Generated at: " + new Date().toDateString());
		autogenerated_text += gen_push_line("*/");
		autogenerated_text += gen_push_line("");

		//------
		//Start header file generation
		//------
		let function_and_type = "varia_resource_get_" + asset_type;
		let refname = "" + asset_type + "_ref";

		header_file += autogenerated_text;
		header_file += gen_push_line("#pragma once");
		header_file += gen_push_line("");
		header_file += gen_push_line("typedef enum");
		header_file += gen_push_line("{");
		for (const elem of image_enums)
		{
			header_file += gen_push_line("\t" + elem + ",");
		}
		header_file += gen_push_line("");
		header_file += gen_push_line("	" + enum_prefix + "MAXIMUM_COUNT");
		header_file += gen_push_line("}");
		header_file += gen_push_line(enum_name + ";");
		header_file += gen_push_line("");
		header_file += gen_push_line("char const * "+ function_and_type + "(" + enum_name + " " + refname + ");");
		header_file += gen_push_line("char const * " + function_and_type + "_enum_name(" + enum_name + " " + refname + ");");
		//-----
		//End header file generation
		//-----



		//------
		//Start object file generation
		//------
		object_file += autogenerated_text;
		object_file += gen_push_line("#include \"assets/" + generated_filename + ".h\"");
		object_file += gen_push_line("");
		object_file += gen_push_line("static char const * " + enum_prefix.toLowerCase() + "enum_names[] = {");
		for (const elem of image_enum_names)
		{
			object_file += gen_push_line("\t" + elem + ",");
		}
		object_file += gen_push_line("};");
		object_file += gen_push_line("");
		object_file += gen_push_line("static char const * " + enum_prefix.toLowerCase() + "paths[] = {");
		for (const elem of image_paths)
		{
			object_file += gen_push_line("\t" + elem + ",");
		}
		object_file += gen_push_line("};");
		object_file += gen_push_line("");
		object_file += gen_push_line("char const * "+ function_and_type + "(" + enum_name + " " + refname + ")");
		object_file += gen_push_line("{");
		// object_file += gen_push_line("	return global_image_resources_paths[image_ref];");
		object_file += gen_push_line("	return " + enum_prefix.toLowerCase() +"paths[" + refname +"];");
		object_file += gen_push_line("}");
		object_file += gen_push_line("");
		object_file += gen_push_line("char const * " + function_and_type + "_enum_name(" + enum_name + " " + refname + ")");
		object_file += gen_push_line("{");
		object_file += gen_push_line("	return " + enum_prefix.toLowerCase() + "enum_names[" + refname + "];");
		object_file += gen_push_line("}");
		object_file += gen_push_line("");
		//------
		//End object file generation
		//------

		//Now, we want to generate the appropriate directory + asset name here, including filetype

		fs.writeFileSync("Sources/assets/" + generated_filename + ".h", header_file);
		fs.writeFileSync("Sources/assets/" + generated_filename + ".c", object_file);
	}
}




//Image resource scan
generate_resource_files(
	"Sources/assets/images/",
	"image",
	[".k", ".png"],
	"VARIA_IMAGE_RESOURCE_",
	"varia_image_resource_e",
	"Autogenerated list of image resources within tectonic.",
	"image_resources"
);

// Other resource scans can begin here
// generate_resource_files(
// 	"Sources/assets/images/",
// 	"music",
// 	[".k"],
// 	"TEC_MUSIC_RESOURCE_",
// 	"tec_music_resource_e",
// 	"Autogenerated list of music",
// 	"music_resources"
// )





/*
***************************************************
***************************************************
*********                                  ********
*********   Actual Project Instantiation   ********
*********                                  ********
***************************************************
***************************************************
*/




let project = new Project('Varia');
project.addFile('Sources/**');
project.addFiles('Sources/shaders/**');
project.addFiles('Sources/assets/**');
project.addIncludeDir('Sources');
project.addDefine('VARIA_LOG_LOGGING_ENABLED=1');
// project.addDefine('VARIA_DISABLE_UNFORCED_ASSERTS=1');
project.setDebugDir('Deployment');

resolve(project);

//Possible flags are:
//TEC_DISABLE_UNFORCED_ASSERTS to disable any non-mandatory assertions
//TEC_DISABLE_TESTS to disable the runtime tests launched before main