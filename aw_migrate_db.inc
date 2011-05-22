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

 //   $this->addFieldMapping('uid', 'gid');
    $this->addFieldMapping('name', 'name');
}

public function prepareRow($current_row) {
}

}

class AWDocMigration extends Migration {
  
  public function __construct() {
  
    $this->dependencies = array('AWUser');
    
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

  }

  public function prepareRow($current_row) {
           
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
  
   



  }

}

function _aw_migrate_convert_file_path($path, $id = NULL, $type, $domain) {  
  $new_path = NULL;
  $path_elements = explode('/', $path);
  $file = str_replace(' ', '+', $path_elements[count($path_elements)-1]);
  switch ($type) {
    case 'file':
      $new_path = $domain . '/orb.aw/class=file/action=preview/id=' . $id . '/' . $file;
      break;
    case 'image':
      $new_path = $domain . '/orb.aw/class=image/action=show/fastcall=1/file=' . $file;
      break;
  }
  return $new_path;
}
