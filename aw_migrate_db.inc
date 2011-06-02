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
      $this->addFieldMapping('field_image_ref', 'related_images')
      ->separator('|');
    }

    if (module_exists('node_reference')) {
      $this->addFieldMapping('field_ref', 'related_files')
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
        $current_row->body = $doc->content;
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
      $current_row->related_images = implode('|' , $images);

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
      $current_row->related_files = implode('|' , $files);
  
  
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
