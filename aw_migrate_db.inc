<?php

class AWUserMigration extends Migration {
  public function __construct() {

    parent::__construct();

    $query = db_select('groups', 'a_g')
             ->fields('a_g', array('gid', 'name'))
             ->condition('type', 1)
             ->distinct();
             
             
    $this->source = new MigrateSourceSQL($query);
    $this->destination = new MigrateDestinationUser();
    
    $this->map = new MigrateSQLMap($this->machineName,
        array('gid' => array(
                'type' => 'int',
                'unsigned' => TRUE,
                'not null' => TRUE,
                )
             ),
        MigrateDestinationUser::getKeySchema()
    );

    $this->addFieldMapping('name', 'name');
}

public function prepareRow($current_row) {
}

}



class AWImageMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
    parent::__construct();
    
    $dir = 'public://aw_images';
    
    if (!file_prepare_directory($dir)) {
      drupal_mkdir($dir);
    }
    
    $query = db_select('objects', 'a_o')
             ->fields('a_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status', 'metadata'))
             ->condition('class_id', 6)
             ->orderBy('modified', 'DESC');
             
    $this->source = new MigrateSourceSQL($query);           
    $this->destination = new MigrateDestinationNode('image');
 
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'oid' => array('type' => 'int',
                        'unsigned' => TRUE,
                        'not null' => TRUE,
                        )
        ),
        MigrateDestinationNode::getKeySchema()
      );
  
    
    $this->addFieldMapping('nid', 'oid');
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    $this->addFieldMapping('title', 'name');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'modified');

    $associated_file_arguments = MigrateFileFieldHandler::arguments(variable_get('file_public_path', conf_path() . '/files/aw_images'), 'file_copy', FILE_EXISTS_RENAME, NULL, 2, 1, NULL, NULL, ';');
    
     $this->addFieldMapping('field_image', 'images')
       ->separator('|')
       ->arguments($associated_file_arguments);
    }

  public function prepareRow($current_row) {
 
 
    $images = array();
    $result = db_select('images', 'aw_i')
      ->fields('aw_i', array('file'))
      ->condition('id', $current_row->oid)
      ->execute();
    if ($result) {
      foreach ($result as $row) {      
        $image = basename($row->file);
      }    
      $current_row->images = $image;
      
      if (!file_exists('public://aw_images/'. $image)) {
  //      $f = file_get_contents(_aw_migrate_convert_file_path($image, 'image'));
  //      file_unmanaged_save_data($f, 'public://aw_images/'. $image, 'FILE_EXISTS_REPLACE');
      }
    
    }
          
     $query = db_select('users', 'u')
        ->fields('u', array('uid'))
        ->condition('name', $current_row->createdby)
        ->execute();
      
     foreach ($query as $user) {
       $current_row->uid = $user->uid;
     }  
   



  }

}




class AWFileMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
    parent::__construct();
    
    $dir = 'public://aw_files';
    
    if (!file_prepare_directory($dir)) {
      drupal_mkdir($dir);
    }
    
    $query = db_select('objects', 'a_o')
             ->fields('a_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status', 'metadata'))
             ->condition('class_id', 41)
             ->orderBy('modified', 'DESC');
             
    $this->source = new MigrateSourceSQL($query);           
    $this->destination = new MigrateDestinationNode('file');
 
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'oid' => array('type' => 'int',
                        'unsigned' => TRUE,
                        'not null' => TRUE,
                        )
        ),
        MigrateDestinationNode::getKeySchema()
      );
  
    
    $this->addFieldMapping('nid', 'oid');
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    $this->addFieldMapping('title', 'name');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'modified');

    $associated_file_arguments = MigrateFileFieldHandler::arguments(variable_get('file_public_path', conf_path() . '/files/aw_files'), 'file_copy', FILE_EXISTS_RENAME, NULL, 2, 1, NULL, NULL, ';');
    
     $this->addFieldMapping('field_file', 'files')
       ->separator('|')
       ->arguments($associated_file_arguments);
    }

  public function prepareRow($current_row) {
 
 
    $files = array();
    $result = db_select('files', 'aw_f')
      ->fields('aw_f', array('file'))
      ->condition('id', $current_row->oid)
      ->execute();
    if ($result) {
      foreach ($result as $row) {      
        $file = basename($row->file);
      }    
      $current_row->files = $file;
      
      if (!file_exists('public://aw_files/'. $file)) {
  //      $f = file_get_contents(_aw_migrate_convert_file_path($file, 'file'));
  //      file_unmanaged_save_data($f, 'public://aw_files/'. $file, 'FILE_EXISTS_REPLACE');
      }
    
    }
          
     $query = db_select('users', 'u')
        ->fields('u', array('uid'))
        ->condition('name', $current_row->createdby)
        ->execute();
      
     foreach ($query as $user) {
       $current_row->uid = $user->uid;
     }  
   



  }

}





class AWDocMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWImage');
    
    parent::__construct();
        
    $query = db_select('objects', 'a_o')
             ->fields('a_o', array('oid', 'name', 'createdby', 'created', 'modified', 'status', 'metadata'))
             ->condition('class_id', 7)
             ->orderBy('modified', 'DESC');
             
    $this->source = new MigrateSourceSQL($query);           
    $this->destination = new MigrateDestinationNode('article');
 
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'oid' => array('type' => 'int',
                        'unsigned' => TRUE,
                        'not null' => TRUE,
                        )
        ),
        MigrateDestinationNode::getKeySchema()
      );
  
    
    $this->addFieldMapping('nid', 'oid');
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('uid', 'uid')->defaultValue(1);
    $this->addFieldMapping('title', 'name');
    $this->addFieldMapping('created', 'created');
    $this->addFieldMapping('changed', 'modified');
    $this->addFieldMapping('body', 'body')->arguments(array('format' => 'full_html'));
    $this->addFieldMapping('path', 'path');
    
    if (module_exists('node_reference')) {
      $this->addFieldMapping('field_reference_image', 'reference_image')
      ->separator('|');
    }

    if (module_exists('node_reference')) {
      $this->addFieldMapping('field_reference_file', 'reference_image')
      ->separator('|');
    }

  }

  public function prepareRow($current_row) {
    
     $current_row->path = 'doc/' . $current_row->oid;
     
     $query = db_select('users', 'u')
        ->fields('u', array('uid'))
        ->condition('name', $current_row->createdby)
        ->execute();
      
     foreach ($query as $user) {
       $current_row->uid = $user->uid;
     }
    
     $query = db_select('documents', 'a_d')
         ->fields('a_d', array('content'))
         ->condition('docid', $current_row->oid)
         ->execute();

      foreach ($query as $doc) {
        $current_row->body =  _aw_migrate_cleanup($doc->content);
   //     $current_row->body =  $doc->content;
      }
  
   
      $images = array();
      $result = db_select('aliases', 'aw_a')
        ->fields('aw_a', array('target'))
        ->condition('source', $current_row->oid)
        ->condition('type', 6)
        ->execute();
      if ($result) {
        foreach ($result as $row) {      
          $images[] = $row->target;
        }    
      }    
      $current_row->reference_image = implode('|' , $images);

      $files = array();
      $result = db_select('aliases', 'aw_a')
        ->fields('aw_a', array('target'))
        ->condition('source', $current_row->oid)
        ->condition('type', 41)
        ->execute();
      if ($result) {
        foreach ($result as $row) {      
          $files[] = $row->target;
        }    
      }    
      $current_row->reference_file = implode('|' , $files);
  
  
  }

}

function _aw_migrate_convert_file_path($path, $type, $domain = NULL, $id = NULL) {  
  $file = basename($path);
  switch ($type) {
    case 'file':
      $new_path = $domain . 'http://www.vonkrahl.ee/orb.aw/class=file/action=preview/id=' . $id . '/' . urlencode($file);
      break;
    case 'image':
      $new_path = $domain . 'http://www.vonkrahl.ee/orb.aw/class=image/action=show/fastcall=1/file=' . $file;
      break;
  }
  return $new_path;
}




function _aw_migrate_cleanup($text) {

/*
    $file_index = NULL;
    $image_index = NULL;
    $gallery_index = NULL;
    
    $found = preg_match_all('#\#file([0-9]+)\##e',$current_row->content_filtered, $matches);
    if ($found) {
      foreach ($matches[1] as $match) {
        $file_index[] = $match;
      }
    }

    $found = preg_match_all('#\#pict([0-9]+)\##e',$current_row->content_filtered, $matches);
    if ($found) {
      foreach ($matches[1] as $match) {
        $image_index[] = $match;
      }
    }

    $found = preg_match_all('#\#minigalerii([0-9]+)\##e',$current_row->content_filtered, $matches);
    if ($found) {
      foreach ($matches[1] as $match) {
        $gallery_index[] = $match;
      }
    }
*/    
    
    $preg_replace_map = array(
      '#\#file([0-9]+)\##e' => '',
      '#\#pict([0-9]+)\##e' => '',
      '#\#minigalerii([0-9]+)\##e' => '',
      '#\s\s+#' => ' ',
      '#<span style=\"font-weight: bold;\">(.*)</span>#e' => "('<strong>$1</strong>')",
      '#<!--(.*)-->#e' => '',
      '#<strong>([\s]+)</strong>#e' => '',
      '#<strong>GALLERY</strong>#e' => '',
    );
    
    $current_row->content_filtered = preg_replace(array_keys($preg_replace_map), array_values($preg_replace_map), $text);


    // Omitting html, body, div, span, fieldset, form, input, select, textarea, label, legend, font

      $allowed_tags = array(
      'applet','object','iframe','h1','h2','h3','h4','h5','h6',
      'p','blockquote','pre','a','abbr','acronym','address','big','cite','code',
      'del','dfn','em','img','ins','kbd','q','s','samp','small','strike',
      'strong','sub','sup','tt','var','b','u','i','center','dl','dt','dd','ol',
      'ul','li', 'table','caption','tbody','tfoot','thead','tr','th','td'
      );

      $text = filter_xss($text, $allowed_tags);

    $replace_map = array(
      '<b>' => '<strong>',
      '</b>' => '</strong>',
      '&nbsp;' => '',
      '<strong></strong>' => '',
      '<p></p>' => '',
      '<p><br />' => '<p>',
      'target="_blank" ' => '',
      '&ndash;' => '-',
      '&rsquo;' => "'",
      '&lsquo;' => "'",
      '&ldquo;' => '"',
      '&rdquo;' => '"',
      '&bdquo;' => '"',
      '&acute;' => '´',
      '&Scaron;' => 'Š',
      '&scaron;' => 'Š',
      '&Otilde;' => 'Õ',
      '&otilde;' => 'õ',
      '&Auml;' => 'Ä',
      '&auml;' => 'ä',
      '&Ouml;' => 'Ö',
      '&ouml;' => 'Ö',
      '&Uuml;' => 'Ü',
      '&uuml;' => 'ü',
    );
  
    $text = str_replace(array_keys($replace_map), array_values($replace_map), $text);

return $text;

}