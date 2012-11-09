<?php
/**
 * @file
 * Páginas adicionas para o módulo Produto.
 */

/**
 * Callback de página.
 * Mostra uma tela para selecionar o tipo de produto a ser criado.
 *
 * @see produto_menu()
 */
function produto_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);
  $count = count($content);

  // Pula esta tela se só existe um tipo de produto.
  if ($count == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }
  // Mostra aviso se não houver nenhum tipo de produto.
  elseif ($count === 0) {
    return t("Você ainda não criou nenhum !types!", array('!types' => l(t('Tipo de Produto'), 'admin/structure/produtos')));
  }

  return theme('node_add_list', array('content' => $content));
}

/**
 * Callback de página.
 * Mostra o formulário de criação de um produto de certo tipo.
 *
 * @see produto_menu()
 */
function produto_add($type) {
  $produto_type = produto_get_types($type);
  $produto = entity_create('produto', array('type' => $type));

  // Verifica se o usuário tem acesso.
  if (!produto_access('create', $produto)) {
    drupal_set_message(t("Você não tem permissão para realizar esta operação."), 'error', FALSE);
    drupal_access_denied();
    exit;
  }

  drupal_set_title(t('Criar Produto: @name', array('@name' => $produto_type->getTranslation('label'))));
  $output = drupal_get_form('produto_form', $produto);
  return $output;
}

/**
 * Callback de formulário.
 * Retorna o formulário de edição de produtos.
 *
 * @see produto_menu()
 */
function produto_form($form, &$form_state, $produto) {
  $form_state['produto'] = $produto;
  $pid = entity_id('produto', $produto);

  // Campo de título.
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Nome'),
    '#default_value' => isset($produto->title) ? $produto->title : '',
    '#required' => TRUE,
  );

  // Campo de SKU.
  $form['sku'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU'),
    '#default_value' => isset($produto->sku) ? $produto->sku : '',
    '#required' => TRUE,
  );

  // Adiciona os fields.
  field_attach_form('produto', $produto, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Salvar produto'),
    '#submit' => $submit + array('produto_form_submit'),
  );

  // Mostra o botão de excluir se o produto existir e se tiermos permissão.
  if (!empty($pid) && produto_access('delete', $produto)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Excluir produto'),
      '#limit_validation_errors' => array(),
      '#submit' => array('produto_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Callback de validação.
 *
 * @see produto_form()
 */
function produto_form_validate(&$form, &$form_state) {
  $produto = $form_state['produto'];
  $values = & $form_state['values'];

  // @TODO validar os dados.
}

/**
 * Callback de submit.
 * Salva um produto.
 *
 * @see produto_form()
 */
function produto_form_submit(&$form, &$form_state) {
  $produto = $form_state['produto'];

  // Constrói a entidade.
  entity_form_submit_build_entity('produto', $produto, $form, $form_state);

  // Salva ela.
  produto_save($produto);

  // Redireciona para ela.
  $produto_uri = entity_uri('produto', $produto);
  $form_state['redirect'] = $produto_uri['path'];

  // Mostra mensagem de sucesso.
  drupal_set_message(t('Produto: %title salvado.', array('%title' => entity_label('produto', $produto))));
}

/**
 * Callback de submit.
 * Redireciona para a tela de confirmação de exclusão de produto.
 *
 * @see produto_form()
 */
function produto_form_submit_delete($form, &$form_state) {
  $produto = $form_state['produto'];
  $produto_uri = entity_uri('produto', $produto);
  $redirect_path = $produto_uri['path'] . '/delete';
  $form_state['redirect'] = $redirect_path;
}

/**
 * Callback de formulário.
 * Retorna o formulário de confirmação de exclusão de produto.
 *
 * @see produto_menu()
 */
function produto_delete_form($form, &$form_state, $produto) {
  $form_state['produto'] = $produto;

  // Sempre fornecemos o id da entidade com a mesma chave que no form de edição.
  $form['pid'] = array('#type' => 'value', '#value' => entity_id('produto', $produto));
  $produto_uri = entity_uri('produto', $produto);
  return confirm_form($form, t('Você tem certeza que deseja excluir o produto: %title?', array('%title' => entity_label('produto', $produto))), $produto_uri['path'], t('Esta ação não pode ser desfeita.'), t('Excluir'), t('Cancelar'));
}

/**
 * Callback de submit.
 * Exclui um produto.
 *
 * @see produto_delete_form()
 */
function produto_delete_form_submit($form, &$form_state) {
  $produto = $form_state['produto'];

  // Exclui o produto.
  produto_delete($produto);

  // Redireciona para a listagem de produtos.
  $form_state['redirect'] = 'admin/content/produto';

  // Mostra mensagem de sucesso.
  drupal_set_message(t('Produto: %title excluído.', array('%title' => entity_label('produto', $produto))));
}