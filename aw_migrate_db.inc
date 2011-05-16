<?php

class AWUserMigration extends Migration {
  public function __construct() {

    parent::__construct();

    $query = db_select('aw_groups', 'a_g')
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
        
    $query = db_select('aw_documents', 'a_d')
             ->fields('a_d', array('docid', 'title', 'content'))
             ->distinct();

    $this->source = new MigrateSourceSQL($query);           
    $this->destination = new MigrateDestinationNode('article');
 
    $this->map = new MigrateSQLMap($this->machineName,
        array(
          'docid' => array('type' => 'int',
                        'unsigned' => TRUE,
                        'not null' => TRUE,
                        )
        ),
        MigrateDestinationNode::getKeySchema()
      );
  
    
    $this->addFieldMapping('nid', 'docid');
    $this->addFieldMapping('is_new')->defaultValue(TRUE);
    $this->addFieldMapping('uid', 'uid');
    $this->addFieldMapping('title', 'title');
    $this->addFieldMapping('created', 'changed');
    $this->addFieldMapping('changed', 'changed');
  //  $this->addFieldMapping('body', 'content')->arguments(array('format' => 'full_html'));
   $this->addFieldMapping('body', 'content_filtered')->arguments(array('format' => 'full_html'));

  }

  public function prepareRow($current_row) {
           
    $query = db_select('aw_config', 'aw_c')
       ->fields('aw_c', array('ckey','modified', 'modified_by'))
       ->condition('ckey', 'document_editor_open_' . $current_row->docid)
       ->execute();
     
     foreach ($query as $cache_key) {
       $current_row->changed = $cache_key->modified;
       $current_row->user_name = $cache_key->modified_by;
     }

     
     if ($current_row->user_name) {
     $query = db_select('users', 'u')
        ->fields('u', array('uid'))
        ->condition('name', $current_row->user_name)
        ->execute();
      
     foreach ($query as $user) {
       $current_row->uid = $user->uid;
     }
    }
  
  
    $current_row->content_filtered =  $current_row->content; 

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
    
    // References "('[file $1]')" 
    
    $preg_replace_map = array(
  //    '#\#file([0-9]+)\##e' => '',
  //    '#\#pict([0-9]+)\##e' => '',
  //    '#\#minigalerii([0-9]+)\##e' => '',
      '#\s\s+#' => ' ',
      '#<span style=\"font-weight: bold;\">(.*)</span>#e' => "('<strong>$1</strong>')",
      '#<!--(.*)-->#e' => '',
      '#<strong>([\s]+)</strong>#e' => '',
      '#<strong>GALLERY</strong>#e' => '',
    );
    
    $current_row->content_filtered = preg_replace(array_keys($preg_replace_map), array_values($preg_replace_map), $current_row->content_filtered);


    // Omitting html, body, div, span, fieldset, form, input, select, textarea, label, legend, font

      $allowed_tags = array(
      'applet','object','iframe','h1','h2','h3','h4','h5','h6',
      'p','blockquote','pre','a','abbr','acronym','address','big','cite','code',
      'del','dfn','em','img','ins','kbd','q','s','samp','small','strike',
      'strong','sub','sup','tt','var','b','u','i','center','dl','dt','dd','ol',
      'ul','li', 'table','caption','tbody','tfoot','thead','tr','th','td'
      );

      $current_row->content_filtered = filter_xss($current_row->content_filtered, $allowed_tags);



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
  
    $current_row->content_filtered = str_replace(array_keys($replace_map), array_values($replace_map), $current_row->content_filtered);

 

     // Files

     $files = array();
       $result = db_select('aw_aliases', 'aw_a')
         ->fields('aw_a', array('source', 'target', 'idx'))
         ->condition('source', $current_row->docid)
         ->condition('type', 41)
         ->execute();
  
       if ($result) {
         foreach ($result as $row) {
           
           $query_files = db_select('aw_files', 'aw_f')
              ->fields('aw_f', array('file'))
              ->condition('id', $row->target)
              ->execute();
            
            foreach ($query_files as $query_file) {
              $file = $query_file->file;
            }
  
            $files[] = _aw_migrate_convert_file_path($file, $row->target, 'file', 'http://www.vonkrahl.ee');
  
         }    
        $current_row->content_filtered .= implode('<br />', $files);
       }
    
    // Images 
    
    
    $files = array();
      $result = db_select('aw_aliases', 'aw_a')
        ->fields('aw_a', array('source', 'target', 'idx'))
        ->condition('source', $current_row->docid)
        ->condition('type', 6)
        ->execute();
 
      if ($result) {
        foreach ($result as $row) {
          
          $query_files = db_select('aw_images', 'aw_i')
             ->fields('aw_i', array('file'))
             ->condition('id', $row->target)
             ->execute();
           
           foreach ($query_files as $query_file) {
             $file = $query_file->file;
           }
        
           $files[] = _aw_migrate_convert_file_path($file, $row->target, 'image', 'http://www.vonkrahl.ee');
          
        }
            
       $current_row->content_filtered .= '<br />' . implode('<br />', $files);
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

// INSERT INTO `acl` (`id`, `gid`, `oid`, `acl`) VALUES

// INSERT INTO `aliases` (`id`, `source`, `target`, `type`, `data`, `idx`, `relobj_id`, `reltype`, `pri`, `cached`) VALUES
       
// INSERT INTO `extlinks` (`id`, `url`, `name`, `hits`, `oid`, `descript`, `newwindow`, `doclinkcollection`, `type`, `docid`) VALUES
